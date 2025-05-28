require 'net/http'
require 'date'
require 'json'
require 'uri'
require 'csv'

class HomeController < ApplicationController
  protect_from_forgery with: :null_session  # Prevent CSRF errors for API-style requests
  
  PORT = "5000"
  IP = "192.168.0.182"
  DATA_FILE_PATH = "/mnt/d/Desktop/Peti/egyetem/Szakdolgozat/WIND_DATA/"
  MATLAB_PATH = "/mnt/c/Peti.C/MATLAB/R2023b/bin/matlab.exe"
  SCRIPT_PATH = "D:/Desktop/Peti/egyetem/Szakdolgozat/scripts"

  def index
  end

  def submit_date
    request.format = :html
  
    from_date = Date.new(
      params[:year_from].to_i,
      params[:month_from].to_i,
      params[:day_from].to_i
    ) rescue nil

    to_date = Date.new(
      params[:year_to].to_i,
      params[:month_to].to_i,
      params[:day_to].to_i
    ) rescue nil

    action = params[:select_action]
    avg = params[:request_avg]
    
    date_data = {
      from_date: from_date,
      to_date: to_date,
      action: action,
      avg: avg
    }    

    # Save this date data as a JSON file (optional)
    File.open(Rails.root.join('public', 'date_data.json'), 'w') do |file|
      file.write(date_data.to_json)
    end

    case action
      when "Download"
        handle_download_response(send_download_request(date_data))
      when "Show"
        handle_show_response(send_show_request(date_data))
    end
  end

  #-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-#

  def send_show_request(date_data)

    remote_server_ip = "http://#{IP}:#{PORT}"
    uri = URI.join(remote_server_ip, "/show") 

    # Append the from_date, to_date and action as query parameters
    uri.query = URI.encode_www_form(from_date: date_data[:from_date].to_s, 
                                    to_date: date_data[:to_date].to_s, 
                                    avg: date_data[:avg].to_s
                                    )
    # Send the GET request to the remote server
    return Net::HTTP.get_response(uri)
  end

  #----------------------#

  def handle_show_response(response)

    if response.is_a?(Net::HTTPSuccess)
      # Parse the response (assuming it's JSON)
      remote_data = JSON.parse(response.body)

      @remote_data = remote_data

      # Pass the remote data to the view

      if !remote_data["data"].empty?
        angle_data = []
        speed_data = []
        date_data = []

        remote_data["data"].each do |entry|
          angle_data += entry["data"]["ANGLE"]
          speed_data += entry["data"]["SPEED"]
          date_data  += entry["data"]["DATE"]
        end

        # Build a hash for the chart (e.g., DATE => SPEED)
        @speed_data = speed_data.each_with_index.map do |speed, i|
          [Time.at(date_data[i].to_f).strftime("%H:%M:%S:%L"), speed]
        end.to_h
        # Build a hash for the chart (e.g., DATE => ANGLE)
        @angle_data = angle_data.each_with_index
                          .select { |angle, i| !angle.nil? }  # Select every 5th date with a non-nil angle
                          .map { |angle, i| [Time.at(date_data[i].to_f).strftime("%H:%M:%S:%L"), angle] }
                          .to_h

        render :response
      else
        flash[:notice] = "No data on remote server."
        redirect_to root_path
      end
    else
      flash[:notice] = "Failed to get data from remote server."
      redirect_to root_path
    end
  end

  #-------------------------------------------------------------#

  def send_download_request(date_data)

    remote_server_ip = "http://#{IP}:#{PORT}"
    uri = URI.join(remote_server_ip, "/download") 

    # Append the from_date, to_date and action as query parameters
    uri.query = URI.encode_www_form(from_date: date_data[:from_date].to_s, 
                                    to_date: date_data[:to_date].to_s
                                    )
    # Send the GET request to the remote server
    return Net::HTTP.get_response(uri)
  end

  #----------------------#
  
  def handle_download_response(response)
    if response.is_a?(Net::HTTPSuccess)
        # Parse the response (assuming it's JSON)
        remote_data = JSON.parse(response.body)

      if !remote_data["data"].empty?
        file_path = DATA_FILE_PATH + "WINDSDATA_#{remote_data["from_date"]}_#{remote_data["to_date"]}.csv"
        
        entries = []

        remote_data["data"].each do |file|
          entries += file["data"]
        end

        # Write to CSV
        CSV.open(file_path, "w") do |csv|
          csv << ["DATE", "SPEED", "ANGLE"]  # CSV header

          entries.each do |entry|
            csv << [entry["DATE"].to_f, entry["SPEED"].to_i, entry["ANGLE"].to_i]
          end
        end

        flash[:notice] = "Data downloaded to #{file_path}"
        if start_matlab = params[:start_matlab]
          run_matlab_plot()
        end
        redirect_to root_path
      else
        flash[:notice] = "No data on remote server."
        redirect_to root_path
      end
    else
      flash[:notice] = "Failed to get data from remote server."
      redirect_to root_path
    end
  end

  #----------------------#

  def run_matlab_plot()
    from_date = Date.new(
      params[:year_from].to_i,
      params[:month_from].to_i,
      params[:day_from].to_i
    ) rescue nil

    to_date = Date.new(
      params[:year_to].to_i,
      params[:month_to].to_i,
      params[:day_to].to_i
    ) rescue nil
    
    matlab = params[:select_matlab]

    command = "addpath('#{SCRIPT_PATH}'); #{matlab}('#{from_date}', '#{to_date}'); exit;"
    system("#{MATLAB_PATH} -r \"#{command}\"")

  end
end

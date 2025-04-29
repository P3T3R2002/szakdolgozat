require 'net/http'
require 'date'
require 'json'
require 'uri'

class HomeController < ApplicationController
  protect_from_forgery with: :null_session  # Prevent CSRF errors for API-style requests
  
  IP = "192.168.0.201"
  PORT = "5000"

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

    avg = params[:request_avg]
    
    date_data = {
      from_date: from_date,
      to_date: to_date,
      avg: avg
    }

    # Save this date data as a JSON file (optional)
    File.open(Rails.root.join('public', 'date_data.json'), 'w') do |file|
      file.write(date_data.to_json)
    end

    # Remote server URL and IP
    # remote_server_ip = "http://#{IP}:#{PORT}" 
    # uri = URI.join(remote_server_ip, "/#{action}")


    remote_server_ip = "http://0.0.0.0:#{PORT}"
    uri = URI.join(remote_server_ip, "/") 

    # Append the from_date and to_date as query parameters
    uri.query = URI.encode_www_form(from_date: from_date.to_s, to_date: to_date.to_s, avg: avg.to_s)

    # Send the GET request to the remote server
    response = Net::HTTP.get_response(uri)

    if response.is_a?(Net::HTTPSuccess)
      # Parse the response (assuming it's JSON)
      remote_data = JSON.parse(response.body)

      @remote_data = remote_data

      # Pass the remote data to the view

      if !remote_data["data"].empty?

        entry = remote_data["data"].first
        angle_data = entry["data"]["ANGLE"]
        speed_data = entry["data"]["SPEED"]
        date_data  = entry["data"]["DATE"]

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
end

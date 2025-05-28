function [windspeed, wind_date, windangle] = get_wind_DATA(fromDate, toDate)
   
   % Format date and filename
    base_path = "D:\Desktop\Peti\egyetem\Szakdolgozat\";

    data_path  = fullfile(base_path, 'WIND_DATA', sprintf('WINDSDATA_%s_%s.csv', fromDate, toDate));
   
   % Read and sort data
    DATA = readtable(data_path, "TextType", "string", "ReadVariableNames", false);
    DATA.Properties.VariableNames = ["DATE", "SPEED", "ANGLE"];

    DATA = sortrows(DATA, "DATE");

    windspeed = round(DATA.SPEED/4095*3000)/100;
    windangle = angle_comparator(DATA.ANGLE/4095*3.3);
    wind_date = DATA.DATE;
    
    % Convert to datetime in Hungary time zone
    wind_date = datetime(wind_date, 'ConvertFrom', 'posixtime', 'TimeZone', 'Europe/Budapest');
end

function angle = angle_comparator(angle)
    for i = 1:size(angle)
        if angle(i) ~= 0
            angle(i) = angle(i)-0.676;
            if angle(i) < 0.225
                angle(i) = 1;
            elseif angle(i) < 0.525
                angle(i) = 2;
            elseif angle(i) < 0.825
                angle(i) = 3;
            elseif angle(i) < 1.125
                angle(i) = 4;
            elseif angle(i) < 1.425
                angle(i) = 5;
            elseif angle(i) < 1.725
                angle(i) = 6;
            elseif angle(i) < 2.025
                angle(i) = 7;
            else 
                angle(i) = 8;
            end
        else
            angle(i) = NaN;
        end
    end
end
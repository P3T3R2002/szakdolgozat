function winds_plot(fromDate, toDate)

    [windspeed, wind_date, windangle] = get_wind_DATA(fromDate, toDate);

    assignin('base', 'wind_date', wind_date);
    assignin('base', 'windspeed', windspeed);
    assignin('base', 'windangle', windangle);
    
    figure(1);
    plot(wind_date(13696:20700), windspeed(13696:20700));
    title('Szélsebesség')
    ylabel('Speed [m/s]')
    figure(2);
    plot(wind_date(13696:20700), windangle(13696:20700), '.');
    title('Szélirány')
    yticklabels({'Észak', 'Észak-Kelet', 'Kelet', 'Dél-Kelet', 'Dél', 'Dél-Nyugat', 'Nyugat', 'Észak-Nyugat'});
    grid;
    ylim([0 9]);

    % (opcionális)
    
    % t = round(size(windspeed)/5);
    % model = 'wind_part';  % Simulink model name
    % load_system(model);  % load the model
    % set_param(model, 'SimulationMode', 'normal');
    % set_param(model, 'StopTime', sprintf('%d', t(1)));  
    % out = sim(model, 'ReturnWorkspaceOutputs', 'on');
end



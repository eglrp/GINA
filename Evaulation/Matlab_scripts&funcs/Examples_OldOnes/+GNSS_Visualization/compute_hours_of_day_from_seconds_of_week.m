function hod = compute_hours_of_day_from_seconds_of_week(sow)

days_sinceGpsWeek = sow/(24.0*3600.); % Days
days_sinceGpsWeek = round(days_sinceGpsWeek,10); % Round to 10th digit due to issues with the floor computation
fulldays = floor(days_sinceGpsWeek); % Problem with floating 
hod = (days_sinceGpsWeek - fulldays)*24;

end
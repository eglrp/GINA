%temporary evaluation script for GNSS Algo


% load('Z:\61_Vehicle_Tests_and_Test_Reports\2015-08-10_EU-Fahrt_Ergebnisse\Daten\Tag-1\iMAR\2015-08-03_Teilstueck1\iMAR_PP_Data.mat')

%%

if resLeastSquaresGpsTime(1)<GPSTime(1)
    startTime = GPSTime(1);
else
    startTime = resLeastSquaresGpsTime(1);
end

if resLeastSquaresGpsTime(end)>GPSTime(end)
    endTime = GPSTime(end);
else
    endTime = resLeastSquaresGpsTime(end);
end



indexStart = find(resLeastSquaresGpsTime<=startTime);
indexEnd = find(resLeastSquaresGpsTime>=endTime);
timeSim = resLeastSquaresGpsTime(indexStart(end):indexEnd(1)); 
posKfEcef = resStandAloneGpsKalmanFilter_x(indexStart(end):indexEnd(1),1:3);
velKfEcef = resStandAloneGpsKalmanFilter_x(indexStart(end):indexEnd(1),4:6);

posLsEcef = resLeastSquaresECEF(indexStart(end):indexEnd(1),1:3);
velLsEcef = resLeastSquaresVelECEF(indexStart(end):indexEnd(1),1:3);
posLsLlh = resLeastSquaresLLH(indexStart(end):indexEnd(1),1:3);


indexStart = find(GPSTime<=startTime);
indexEnd = find(GPSTime>=endTime);
timeImar = (interp1(GPSTime(indexStart(end):indexEnd(1)),GPSTime(indexStart(end):indexEnd(1)),timeSim));
velImarEnu = [ (interp1(GPSTime(indexStart(end):indexEnd(1)),VEast(indexStart(end):indexEnd(1)),timeSim)) (interp1(GPSTime(indexStart(end):indexEnd(1)),VNorth(indexStart(end):indexEnd(1)),timeSim)) (interp1(GPSTime(indexStart(end):indexEnd(1)),VUp(indexStart(end):indexEnd(1)),timeSim)) ];
posImarLlh = [ (interp1(GPSTime(indexStart(end):indexEnd(1)),Latitude(indexStart(end):indexEnd(1)),timeSim)) (interp1(GPSTime(indexStart(end):indexEnd(1)),Longitude(indexStart(end):indexEnd(1)),timeSim)) (interp1(GPSTime(indexStart(end):indexEnd(1)),HEll(indexStart(end):indexEnd(1)),timeSim)) ];





%% KF eval
posKfLlh = zeros(length(timeSim),3);
for i=1:length(timeSim)
[posKfLlh(i,1), posKfLlh(i,2), posKfLlh(i,3)] = ecef2llh(posKfEcef(i,1),posKfEcef(i,2),posKfEcef(i,3));
end

velKfEnu = zeros(length(velKfEcef(:,1)),3);
for i=1:length(velKfEcef(:,1))
    
F = getXyz2EnuMat(Latitude(i), Longitude(i));

velKfEnu(i,:) = F*[velKfEcef(i,1); velKfEcef(i,2); velKfEcef(i,3)];

velKfEnu_abs(i) = norm(velKfEnu(i,:));

end


%% LS preprocessing
velLsEnu = zeros(length(timeSim),3);
for i=1:length(timeSim)
    
F = getXyz2EnuMat(posImarLlh(i,1), posImarLlh(i,2));

velLsEnu(i,:) = F*[velLsEcef(i,1); velLsEcef(i,2); velLsEcef(i,3)];

velLsEnu_abs(i) = norm(velLsEnu(i,:));

end

posImarEcef = zeros(length(timeSim),3);
for i=1:length(timeSim)
    [posImarEcef(i,1), posImarEcef(i,2), posImarEcef(i,3)] = llh2ecef(posImarLlh(i,1),posImarLlh(i,2),posImarLlh(i,3));
end


posLsErrEnu = zeros(length(timeSim),3);
posKfErrEnu = zeros(length(timeSim),3);
for i=1:length(timeSim)
    
    F = getXyz2EnuMat(posImarLlh(i,1), posImarLlh(i,2));

    posLs = F*[posLsEcef(i,1); posLsEcef(i,2); posLsEcef(i,3)];
    posKf = F*[posKfEcef(i,1); posKfEcef(i,2); posKfEcef(i,3)];
    posImar = F*[posImarEcef(i,1); posImarEcef(i,2); posImarEcef(i,3)];
    
    posLsErrEnu(i,:) = posLs - posImar;
    posKfErrEnu(i,:) = posKf - posImar;


end




%% Plots absolute


figure(4)
hold off
plot(timeSim,posImarLlh(:,1),'g')
hold on
plot(timeSim,posLsLlh(:,1),'r')
plot(timeSim,posKfLlh(:,1),'b')
legend('iMAR','LS','KF')


figure(5)
hold off
plot(timeSim,posImarLlh(:,2),'g')
hold on
plot(timeSim,posLsLlh(:,2),'r')
plot(timeSim,posKfLlh(:,2),'b')
legend('iMAR','LS','KF')


figure(6)
hold off
plot(timeSim,posImarLlh(:,3),'g')
hold on
plot(timeSim,posLsLlh(:,3),'r')
plot(timeSim,posKfLlh(:,3),'b')
legend('iMAR','LS','KF')


%%%%%%%%%%%%%%%
figure(4)
hold off
plot(timeSim,posImarEcef(:,1)-posImarEcef(2,1),'g')
hold on
plot(timeSim,posLsEcef(:,1)-posImarEcef(2,1),'r')
plot(timeSim,posKfEcef(:,1)-posImarEcef(2,1),'b')
legend('iMAR','LS','KF')

figure(5)
hold off
plot(timeSim,posImarEcef(:,2)-posImarEcef(2,2),'g')
hold on
plot(timeSim,posLsEcef(:,2)-posImarEcef(2,2),'r')
plot(timeSim,posKfEcef(:,2)-posImarEcef(2,2),'b')
legend('iMAR','LS','KF')

figure(6)
hold off
plot(timeSim,posImarEcef(:,3)-posImarEcef(2,3),'g')
hold on
plot(timeSim,posLsEcef(:,3)-posImarEcef(2,3),'r')
plot(timeSim,posKfEcef(:,3)-posImarEcef(2,3),'b')
legend('iMAR','LS','KF')

%%%%%%%%%%%%%%%



figure(5)
hold off
plot(timeSim,velImarEnu(:,1),'g')
hold on
plot(timeSim,velLsEnu(:,1),'r')
plot(timeSim,velKfEnu(:,1),'b')
legend('iMAR','LS','KF')

figure(6)
hold off
plot(timeSim,velImarEnu(:,2),'g')
hold on
plot(timeSim,velLsEnu(:,2),'r')
plot(timeSim,velKfEnu(:,2),'b')
legend('iMAR','LS','KF')

figure(7)
hold off
plot(timeSim,velImarEnu(:,3),'g')
hold on
plot(timeSim,velLsEnu(:,3),'r')
plot(timeSim,velKfEnu(:,3),'b')
legend('iMAR','LS','KF')


velImarEnu_abs=sqrt(velImarEnu(:,1).^2+velImarEnu(:,2).^2+velImarEnu(:,3).^2);

figure(8)
hold off
plot(timeSim,velImarEnu_abs,'g')
hold on
plot(timeSim,velLsEnu_abs,'r')
plot(timeSim,velKfEnu_abs,'b')
legend('iMAR','LS','KF')


%% Plots error

figure(42)
hold off
plot(timeSim,posLsErrEnu(:,1),'r')
hold on
plot(timeSim,posKfErrEnu(:,1),'b')
legend('LS','KF')


figure(52)
hold off
plot(timeSim,posLsErrEnu(:,2),'r')
hold on
plot(timeSim,posKfErrEnu(:,2),'b')
legend('LS','KF')


figure(62)
hold off
plot(timeSim,posLsErrEnu(:,3),'r')
hold on
plot(timeSim,posKfErrEnu(:,3),'b')
legend('LS','KF')



figure(4)
hold off
plot(timeSim,velLsEnu(:,1)-velImarEnu(:,1),'r')
hold on
plot(timeSim,velKfEnu(:,1)-velImarEnu(:,1),'b')
legend('LS','KF')


figure(5)
hold off
plot(timeSim,velLsEnu(:,2)-velImarEnu(:,2),'r')
hold on
plot(timeSim,velKfEnu(:,2)-velImarEnu(:,2),'b')
legend('LS','KF')


figure(6)
hold off
hold off
plot(timeSim,velLsEnu(:,3)-velImarEnu(:,3),'r')
hold on
plot(timeSim,velKfEnu(:,3)-velImarEnu(:,3),'b')
legend('LS','KF')


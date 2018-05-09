%temporary evaluation script for GNSS Algo


% load('Z:\61_Vehicle_Tests_and_Test_Reports\2015-08-10_EU-Fahrt_Ergebnisse\Daten\Tag-1\iMAR\2015-08-03_Teilstueck1\iMAR_PP_Data.mat')

%%





posEcefStation = [1112777.3300 -4341475.8300  4522955.7800];
[posLlhStation(1), posLlhStation(2), posLlhStation(3)] = ecef2llh(posEcefStation(1),posEcefStation(2),posEcefStation(3));


timeSim = resObsTime;
posKfEcef = resStandAloneGpsKalmanFilter_x;
velKfEcef = resStandAloneGpsKalmanFilter_x;

posLsEcef = resLeastSquaresECEF;
velLsEcef = resLeastSquaresVelECEF;
posLsLlh = resLeastSquaresLLH;

posEcefRTK = zeros(length(SingleCodeOnly(:,2)),3);
for i=1:length(SingleCodeOnly(:,2))
    [posEcefRTK(i,1),posEcefRTK(i,2),posEcefRTK(i,3)] = llh2ecef(SingleCodeOnly(i,3)/180*pi,SingleCodeOnly(i,4)/180*pi,SingleCodeOnly(i,5));
end

%% KF eval
posKfLlh = zeros(length(timeSim),3);
for i=1:length(timeSim)
[posKfLlh(i,1), posKfLlh(i,2), posKfLlh(i,3)] = ecef2llh(posKfEcef(i,1),posKfEcef(i,2),posKfEcef(i,3));
end

for i=1:length(timeSim)
    dy_abs(i) = norm(resStandAloneGpsKalmanFilter_dy(i,:));
end

figure(1)
hold off
plot(SingleCodeOnly(:,2),SingleCodeOnly(:,5),'g')
hold on
plot(timeSim, posLsLlh(:,3), 'b' )
plot(timeSim, posKfLlh(:,3), 'r' )



figure(2)
hold off
% plot(resObsTime(:,1),numProcessedPrMes)
hold on
plot(SingleCodeOnly(:,2),SingleCodeOnly(:,7),'g')

figure(3)
plot(timeSim,dy_abs);




posLsErrEnu = zeros(length(timeSim),3);
posKfErrEnu = zeros(length(timeSim),3);
for i=1:length(timeSim)
    
    F = getXyz2EnuMat(posLlhStation(1), posLlhStation(2));

    posLs = F*[posLsEcef(i,1); posLsEcef(i,2); posLsEcef(i,3)];
    posKf = F*[posKfEcef(i,1); posKfEcef(i,2); posKfEcef(i,3)];
    posStation = F*[posEcefStation(1); posEcefStation(2); posEcefStation(3)];
    
    posLsErrEnu(i,:) = posLs - posStation;
    posKfErrEnu(i,:) = posKf - posStation;


end



%% Plots absolute

n=length(timeSim);
stat = ones(n,1);
figure(4)
hold off
plot(timeSim,stat*posLlhStation(1),'g')
hold on
plot(timeSim,posLsLlh(:,1),'r')
plot(timeSim,posKfLlh(:,1),'b')
legend('iMAR','LS','KF')

figure(5)
hold off
plot(timeSim,stat*posLlhStation(2),'g')
hold on
plot(timeSim,posLsLlh(:,2),'r')
plot(timeSim,posKfLlh(:,2),'b')
legend('iMAR','LS','KF')


figure(6)
hold off
plot(timeSim,stat*posLlhStation(3),'g')
hold on
plot(timeSim,posLsLlh(:,3),'r')
plot(timeSim,posKfLlh(:,3),'b')
legend('iMAR','LS','KF')


%%%%%%%%%%%%%%%
figure(43)
hold off
plot(timeSim,stat*posEcefStation(1)-posEcefStation(1),'k')
hold on
plot(SingleCodeOnly(:,2),posEcefRTK(:,1)-posEcefStation(1),'g')
plot(timeSim,posLsEcef(:,1)-posEcefStation(1),'r')
plot(timeSim,posKfEcef(:,1)-posEcefStation(1),'b')
legend('', 'RTKlib','LS','KF')

figure(53)
hold off
plot(timeSim,stat*posEcefStation(2)-posEcefStation(2),'k')
hold on
plot(SingleCodeOnly(:,2),posEcefRTK(:,2)-posEcefStation(2),'g')
plot(timeSim,posLsEcef(:,2)-posEcefStation(2),'r')
plot(timeSim,posKfEcef(:,2)-posEcefStation(2),'b')
legend('', 'RTKlib','LS','KF')

figure(63)
hold off
plot(timeSim,stat*posEcefStation(3)-posEcefStation(3),'k')
hold on
plot(SingleCodeOnly(:,2),posEcefRTK(:,3)-posEcefStation(3),'g')
plot(timeSim,posLsEcef(:,3)-posEcefStation(3),'r')
plot(timeSim,posKfEcef(:,3)-posEcefStation(3),'b')
legend('', 'RTKlib','LS','KF')

%% Plots error

figure(43)
hold off
plot(timeSim,posLsErrEnu(:,1),'r')
hold on
plot(timeSim,posKfErrEnu(:,1),'b')
legend('LS','KF')


figure(53)
hold off
plot(timeSim,posLsErrEnu(:,2),'r')
hold on
plot(timeSim,posKfErrEnu(:,2),'b')
legend('LS','KF')


figure(63)
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


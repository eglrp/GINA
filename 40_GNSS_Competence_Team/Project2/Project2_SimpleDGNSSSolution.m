%% This script tries to calculate a simple PR based solution 
% GNSS Competence Team Project 1
% Simple Iterative Least Squares based navigation solution example

%% INIT

clc;clear all;
close all

Time = 1;
x           = [0, 0, 6390, 0];                  % User position and clk error in state-vector
RoverPos    = [0, 0, 6400];                     % Real position, expected result

BasePos = [0, 0, 6350];                     % Base Station Position

Pos1 = [ 6400, -6400, 22000];                   % From navigation messages
Pos2 = [-6400,  6400, 26000];
Pos3 = [-6400, -6400, 20000];
Pos4 = [ 6400,  6400, 25000];

convlimit = 3e-3;                               % Residual threshold (convergence is minimized)

SatStore{1} = Pos1;                             % Structure to hold the positions
SatStore{2} = Pos2;
SatStore{3} = Pos3;
SatStore{4} = Pos4;
PRObservationsRover = [];                            % Array initializations
PRObservationsBase = [];
geometryDistance = [];   

numSats = size(SatStore,2); 

%% Calculate Pseudoranges
for i=1:numSats
   % Initial geometric distance
   PRObservationsRover(i) = sqrt(sum((RoverPos-SatStore{i}).^2)); 
   PRObservationsBase(i) = sqrt(sum((BasePos-SatStore{i}).^2)); 
   
   % Add Errors
   % Clock bias
   PRObservationsRover(i) = PRObservationsRover(i) + 200;
   PRObservationsBase(i) = PRObservationsBase(i) + 50;
   % Random error
%      PRObservations(i) = PRObservations(i) + randi([50 200]);
end

figure;
hold on;
title('Solution values during the iteration');
xlabel('Iteration number');
ylabel('Iterated value');
%% Iterate navigation solutions
% No SV clock error, No Iono/Tropo/Group and signal travel time errors
% Just rcv clock error
%
% Initial guess sol__ = [0,0,0]
%
sol(1) = 0;
sol(2) = 0;
sol(3) = 0;
sol(4) = 0; %Clock

% Start Iteration --------------------------------
for it = 1:2000             % Should be a 'while' but possible to have infinite 
                            % loops when playing with initial values/errors
    
    % Create Design matrix ---------------------------
    for i = 1:numSats

        % Recalculate geometry distance [ y = ^y - y_corr ]in each iteration 
        geometryDistanceRover(i) = sqrt(sum((BasePos(1:3)+sol(1:3)-SatStore{i}).^2));
        geometryDistanceBase(i) = sqrt(sum((BasePos(1:3)-SatStore{i}).^2));
        % Create the Design Matrix [A]
        for j = 1:3
            rho = sqrt(sum((BasePos(1:3)+sol(1:3) - SatStore{1}(1:3)).^2));
            aij = (BasePos(j)+sol(j)-SatStore{i}(j))/rho;
            A(i,j) = aij;
        end
        A(i,4) = 1.0;       % 4th column is the clock bias, all 1
    end

    % Create covariance matrix -----------------------
    if(det(A.'*A)> 1e-3)
        Cov = inv(A.'*A);              
    else
        Cov = pinv(A.'*A);  % Moore-Penrose pseudoinverse
    end
    
    % Multiply Covariance matrix with A.' and PR residual matrix
    y = PRObservationsRover - PRObservationsBase - geometryDistanceRover + geometryDistanceBase; %Initial PR - iterated one residual
    x = Cov*A'*y';

    % Add delta solution, if x' is less than threshold, we have a final
    % solution ---------------------------------------
    sol(1:3) = sol(1:3) + x(1:3)';
    sol(4) = x(4);

    convergence = norm(x(1:3));
    if(convergence < convlimit)
        disp('Converged');
        
        break;
    end

    % This plot shows the iteration of the solution
    stem(it,norm(BasePos(1:3)+sol(1:3)-RoverPos(1:3)));

end
disp('Position');
BasePos(1:3) + sol(1:3)
disp('Reciever clock offset');
sol(4)
disp('Position error');
norm(BasePos(1:3)+sol(1:3)-RoverPos(1:3))



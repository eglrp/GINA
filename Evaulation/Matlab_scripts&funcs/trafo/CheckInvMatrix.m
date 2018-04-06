function Invertibility = CheckInvMatrix(A)

% Function to check invertibility of Matrix
%
% The function checks if the determinante of the Matrix A is unequal to
% '0'.
% In this case Invertibility is 'true' else 'false'

Invertibility = (det(A)~=0); 

end
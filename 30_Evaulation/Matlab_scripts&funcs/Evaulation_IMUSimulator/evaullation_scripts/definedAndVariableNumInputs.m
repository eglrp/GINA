function definedAndVariableNumInputs(X,Y,varargin)
    disp("Total number of input arguments: " + nargin)
    varargin(1)
    formatSpec = "Size of varargin cell array: %dx%d";
    str = compose(formatSpec,size(varargin));
    disp(str)

end
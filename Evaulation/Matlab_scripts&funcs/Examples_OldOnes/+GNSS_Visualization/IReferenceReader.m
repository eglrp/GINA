classdef IReferenceReader < handle

    methods(Abstract)
        Ref = Read(this,varargin)
    end
    
end
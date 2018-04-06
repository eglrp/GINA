classdef LogicMask < GNSS_Visualization.ILogicMask
    
    properties
        dummyMask = [];
    end
    methods (Access = public)
        function this = LogicMask(varargin)
            if nargin >=1
                this.dummyMask = varargin{1}; 
            end
        end
        
        function setDummyMask(this,dummyMask)
            this.dummyMask = dummyMask;
        end
        
        function out = getDummyMask(this)
            out = this.dummyMask;
        end
        
    end
end
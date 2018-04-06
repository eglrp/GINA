classdef NullReference < GNSS_Visualization.IReference
    
    methods (Access = public)
        function this = NullReferenc(varargin)

        end
        
        function result = GetRefGPSSoW(this,varargin)
            result = [];
        end
        function result = GetRefGPSWeek(this,varargin)
            result = [];
        end
        function result = GetRefLatRad(this,varargin)
            result = [];
        end
        function result = GetRefLonRad(this,varargin)
            result = [];
        end
        function result = GetRefHigh(this,varargin)
            result = [];
        end
        
        function result = GetRefLatDeg(this,varargin)
            result = [];
        end
        function result = GetRefLonDeg(this,varargin)
            result = [];
        end

        function result = GetRefX(this,varargin)
            result = [];
        end
        function result = GetRefY(this,varargin)
            result = [];
        end
        function result = GetRefZ(this,varargin)
            result = [];
        end
    end
    
end
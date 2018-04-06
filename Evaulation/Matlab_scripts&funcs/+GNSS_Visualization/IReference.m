classdef IReference < handle
    
    properties 
        GPSWeek = [];
        GPSSoW  = [];
        LatRad  = [];
        LonRad  = [];
        LatDeg  = [];
        LonDeg  = [];
        High	= [];
        X       = [];
        Y       = [];
        Z       = [];
    end
    methods (Abstract)
        
        result = GetRefGPSSoW(this,varargin)
        result = GetRefGPSWeek(this,varargin)
        
        result = GetRefLatRad(this,varargin)
        result = GetRefLonRad(this,varargin)
        result = GetRefHigh(this,varargin)
        
        result = GetRefLatDeg(this,varargin)
        result = GetRefLonDeg(this,varargin)
        
        result = GetRefX(this,varargin)
        result = GetRefY(this,varargin)
        result = GetRefZ(this,varargin)
    end
    
    methods
       function set.GPSWeek(this,data)
            this.GPSWeek = data;
        end
        function set.GPSSoW(this,data)
            this.GPSSoW = data;
        end
        function set.LatRad(this,data)
            this.LatRad = data;
        end
        function set.LonRad(this,data)
            this.LonRad = data;
        end
        
        function set.LatDeg(this,data)
            this.LatDeg = data;
        end
        function set.LonDeg(this,data)
            this.LonDeg = data;
        end
        function set.High(this,data)
            this.High = data;
        end
        function set.X(this,data)
            this.X = data;
        end
        function set.Y(this,data)
            this.Y = data;
        end
        function set.Z(this,data)
            this.Z = data;
        end 
    end
end
classdef kinematicReference < GNSS_Visualization.IReference
   
    methods 
        function this =  kinematicReference(varargin)
            switch nargin
                case 1
                    if isstruct(varargin{1}) 
                    end
                otherwise
                    
            end
        end
        
        function result = GetRefGPSWeek(this,varargin)
           	result = this.GPSWeek;
        end
        function result = GetRefGPSSoW(this,varargin)    	
           result = this.GPSSoW;
        end
        function result = GetRefLatRad(this,varargin)
            result = this.LatRad;
        end
        function result = GetRefLonRad(this,varargin)
            result = this.LonRad;
        end
        function result = GetRefHigh(this,varargin)
            result = this.High;
        end
        
        function result = GetRefLatDeg(this,varargin)
            result = this.LatDeg;
        end
        function result = GetRefLonDeg(this,varargin)
            result = this.LonDeg;
        end
        
        function result = GetRefX(this,varargin)
            result = this.X;
        end
        function result = GetRefY(this,varargin)
            result = this.Y;
        end
        function result = GetRefZ(this,varargin)
            result = this.Z;
        end
        
     
    end
end
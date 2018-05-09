classdef staticReference < GNSS_Visualization.IReference
    
    
    methods
        function this =  staticReference(varargin)
            coordinate = this.InputParser(varargin{:});
            if ~isempty(coordinate)
                try 
                    this.X = coordinate(1);
                  	this.Y = coordinate(2);
                  	this.Z = coordinate(3);
            	catch ME
                    warning(ME.message)
            	end
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
    
    methods (Access = private)
        
        function coordinate = InputParser(this,varargin)
            coordinate = [];
            if nargin >=2
                if ischar(varargin{1})
                    inputref = str2num(varargin{1});
                else 
                    inputref = varargin{1};
                end
                    if isvector(inputref)&& all(size(inputref) == [1, 3])
                    	coordinate = inputref;
                    end     	
            end
            
        end
    end
end
classdef ReferenceReader_static_w_VMPS_reference < GNSS_Visualization.IReferenceReader
   
    methods 
        function Ref = Read(this,varargin)
            if nargin >=1 
                Ref = read_VMPS_ref(this,varargin{1});
            else
                Ref = read_VMPS_ref(this,[]);
            end
           	if sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(Ref)))==0
                Ref = GNSS_Visualization.NullReference();
            end
        
        end
    end

    methods (Access = private)
        function VMPS_ref = read_VMPS_ref(this,varargin)
            reference   = [];
            filename    = [];
            if ~isempty(varargin{1}) && ischar(varargin{1})
                filename    =  which(varargin{1}); 
                if isempty(filename)
                    filename    =  fullfile(pwd,varargin{1});
                end
            end
           	if ~isempty(filename)
             	[pathstr,name,ext] = fileparts(filename);
                
                    try
                        reference = load([filename]); 
                        VMPS_ref = GNSS_Visualization.kinematicReference();
                        
                        VMPS_ref.GPSSoW     = reference.RXM_RAW2_Abs_header_recTow'; 
                        VMPS_ref.GPSWeek    = reference.RXM_RAW2_Abs_header_week';
                        
                        VMPS_ref.LatRad     = reference.Pos_G_GlobalPosition_lat';
                        VMPS_ref.LonRad     = reference.Pos_G_GlobalPosition_lon';
                        VMPS_ref.High       = reference.Pos_G_GlobalPosition_h';
                        VMPS_ref.LatDeg     = reference.Pos_G_GlobalPosition_lat'*180/pi;
                        VMPS_ref.LonDeg     = reference.Pos_G_GlobalPosition_lon'*180/pi;

                        VMPS_ref.X          = reference.Pos_ECEF_GlobalPosition_ECEF_x';
                        VMPS_ref.Y       	= reference.Pos_ECEF_GlobalPosition_ECEF_y';
                        VMPS_ref.Z          = reference.Pos_ECEF_GlobalPosition_ECEF_z';
                   
                    catch ME
                      	warning(ME.message)
                        reference = [];
                    end

            end
            if isempty(reference)
               VMPS_ref = GNSS_Visualization.NullReference(); 
            end
        end
    end    
end
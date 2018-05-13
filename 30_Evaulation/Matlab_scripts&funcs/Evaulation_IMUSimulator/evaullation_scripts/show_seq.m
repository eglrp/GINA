function show_seq(varargin)
  hold on %# make sure subsequent plots don't overwrite the figure
  colors = 'rb'; %# define more colors here, 
                 %# or use distingushable_colors from the
                 %# file exchange, if you want to plot more than two

  %# loop through the inputs and plot
  for iArg = 1:nargin
      plot(varargin{iArg},'color',colors(iArg));
  end
end
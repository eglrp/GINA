function myArray = saveElementsofArraybyGivenIndeces(myArray,idx)
    if ~isempty(myArray)
        myArray = myArray(idx);
    else
        disp(['WARNING: <a href="matlab:opentoline(which(''GNSS_Visualization.visualize''),562,0);">GNSS_Visualization::visualize</a>: The given array is empty.'])
    end
end
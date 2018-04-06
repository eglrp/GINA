function R3 = get_rotationMatrix_R3(a)

    R_3 = [ cos(a),sin(a), 0;
           -sin(a),cos(a), 0;
                 0,     0, 1];
    
    R3 = R_3;
end
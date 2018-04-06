function R1 = get_rotationMatrix_R1(a)

    R_1 = [ 1,    0,     0;
            0, cos(a),sin(a);
            0,-sin(a),cos(a)];
    
    R1 = R_1;
end
function R2 = get_rotationMatrix_R2(a)

    R_2 = [ cos(a),0,-sin(a);
            0,     1,      0;
            sin(a),0,  cos(a)];
    
    R2 = R_2;
end
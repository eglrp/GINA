function [R1, R2, R3] = get_rotationMatrices(a)

    R_1 = [1,0,0;0,cos(a),sin(a);0,-sin(a),cos(a)];
    R_2 = [cos(a),0,-sin(a);0,1,0;sin(a),0,cos(a)];
    R_3 = [cos(a),sin(a),0;-sin(a),cos(a),0;0,0,1];
    
    R1 = R_1;
    R2 = R_2;
    R3 = R_3;
end
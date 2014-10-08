function generateTest

obj = 'sphere';

r = 0.01; 
N = 20;
N=5;

A = zeros(N^2,3); % r,phi,theta

% phi   = linspace(0    ,2*pi,2*N); 
% theta = linspace(-pi/2,pi/2,N);
phi   = linspace(0,0.1,N); 
theta = linspace(0,0.1,N);


[PHI,THETA] = meshgrid(phi,theta); 

x = r.*cos(THETA).*sin(PHI); 
y = r.*sin(THETA); 
z = r.*cos(THETA).*cos(PHI); 

A(:,1) = reshape(x,N^2,1);
A(:,2) = reshape(y,N^2,1);
A(:,3) = reshape(z,N^2,1);

figure, plot3(A(:,1),A(:,2),A(:,3),'x'), axis equal



save('-ascii',[obj,'.txt'],'A');



end



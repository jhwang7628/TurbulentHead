function [A,T,Pv,player] = compactFarFieldSound(wc)

addpath('~/Research/AerodynamicSound/matlab')
% wc: cut off frequency for smoothing before differentiation. 
% this smoothing is done using second order butterworth filter with cut off frequency wc

%  * ri: observer position
%  * scaling: scaling to the final sound rendering


% Far-field specification
ri = 0.25;
extension = 1;
c0 = 340; 
scaling = 1;

% Read-in Sim files
filename1 = 'ForceCoefficients/cd-1-history'
filename2 = 'ForceCoefficients/cl-1-history'
filename3 = 'ForceCoefficients/cz-1-history'
% filename1 = input('Please input the x-force data location wrapped with quotation or press [ENTER] for zeros: ')
% filename2 = input('Please input the y-force data location wrapped with quotation or press [ENTER] for zeros: ')
% filename3 = input('Please input the z-force data location wrapped with quotation or press [ENTER] for zeros: ')

if isempty(filename1) && isempty(filename2) && isempty(filename3)
    fprint('ERROR: You did not input any filenames!\n')
    return; 
end

if ~isempty(filename1) 
    drag_file1 = dlmread(filename1,'',2,0);
    T = drag_file1(:,1); 
    drag_file_len = size(drag_file1,1);
end
if ~isempty(filename2) 
    drag_file2 = dlmread(filename2,'',2,0);
    T = drag_file2(:,1); 
    drag_file_len = size(drag_file2,1);
end
if ~isempty(filename3) 
    drag_file3 = dlmread(filename3,'',2,0);
    T = drag_file3(:,1); 
    drag_file_len = size(drag_file3,1);
end

if isempty(filename1)
       drag_file1 = [T,zeros(drag_file_len,1)];
end
if isempty(filename2)
       drag_file2 = [T,zeros(drag_file_len,1)];
end
if isempty(filename3)
       drag_file3 = [T,zeros(drag_file_len,1)];
end


A = [drag_file1(:,1:2), drag_file2(:,2), drag_file3(:,2)];

cutoff = 500; 

A = A(cutoff:end,:);


if exist('wc') 
    SampFreq = floor(1/(A(2,1)-A(1,1))); 
    A(:,2:end) = MyFilter(A(:,2:end),wc./(0.5.*SampFreq),1);
    dropdata = 1;  % after the filter needs to drop some errie data
    A = A(dropdata:end,:);
end

A(:,1) = A(:,1) - A(1,1); % Shift the drag start time back to zero.
dt = A(end,1)-A(end-1,1);

T        = A(cutoff:end-1,1  ); 
  Forces = A(cutoff:end-1,2:4);
dtForces =(A(cutoff+1:end,2:4) - A(cutoff-1:end-2,2:4))/(2*dt);



Pv    = ri.*sum(dtForces,2)./(4*pi*c0*norm([ri,ri,ri]).^2);
SPL_t = calc_SPL(Pv);

[f,fftPv] = PlotFFTSignal(T,Pv,0);
Pv_f_PSD  = sqrt(real(fftPv).^2 + imag(fftPv).^2);
SPL_f = calc_SPL(Pv_f_PSD,1); 


if exist('extension')
   Pvtmp=[];
   for ii = 1:extension
      Pvtmp = [Pvtmp;Pv];
   end
   Pv=Pvtmp;
end


% Sound Rendering
player = audioplayer(Pv*scaling,floor(1/dt));

audiowrite('out/CompactFarField.wav', Pv/max(Pv), floor(1/dt));




end


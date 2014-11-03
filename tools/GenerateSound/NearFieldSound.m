% function NearFieldSound 
%
tic
data_read = 1

if data_read == 1
    fprintf('reading source1.txt\n');
    s1 = load('out/source1.txt');
    fprintf('reading source2.txt\n');
    s2 = load('out/source2.txt');
    
    HRIR_folder = '/home/jui-hsien/Research/Ansys-run/HumanHead-2/HRIR/data_head/';
    
    FileLsL = dir([HRIR_folder, 'head_left_*']);
    FileLsR = dir([HRIR_folder, 'head_right_*']);
    
    fprintf('probing HRIR dataset\n')
    probe = dlmread([HRIR_folder,FileLsL(1).name],'',1,0);
    NCell = length(probe);
    Nts   = length(FileLsL);
    clear probe;
    
    HRIR = zeros(NCell,Nts,2); % 1 is left and 2 is right
    
    for ii = 1:length(FileLsL)
        fprintf('reading file %s\n', FileLsL(ii).name);
        HRIR(:,ii,1) = dlmread([HRIR_folder,FileLsL(ii).name],'',1,0);
        % tmp = dlmread([HRIR_folder,FileLsL(ii).name],'',1,0);
        fprintf('reading file %s\n', FileLsR(ii).name);
        HRIR(:,ii,2) = dlmread([HRIR_folder,FileLsR(ii).name],'',1,0);
    end

end

%upsampling
%
ratio = ceil(44000/5000);
t = linspace(0,1/5000*size(s1,2),size(s1,2));
ti= linspace(0,1/5000*size(s1,2),size(s1,2)*ratio);
s1interp = zeros(NCell, size(s1,2)*ratio);
sound_1 = zeros(NCell,size(s1,2)*ratio,2);
for ii = 1:NCell
    fprintf('upsamping and convoluting cell %i for source 1\n', ii);
    s1interp(ii,:) = interp1(t,s1(ii,:),ti,'spline');
    % fprintf('convoluting cell %i\n', ii);
    sound_1(ii,:,1) = conv(s1interp(ii,:),HRIR(ii,:,1),'same');
    sound_1(ii,:,2) = conv(s1interp(ii,:),HRIR(ii,:,2),'same');
end

clear s1 s1interp;

toc
tic
fprintf('saving matlab workspace\n')
save('-v7.3','sound_1.mat','sound_1')

SUM_sound_1 = squeeze(sum(sound_1,1));
fprintf('writing sound\n')
audiowrite('sound_1.wav',SUM_sound_1/max(max(abs(SUM_sound_1))),44000);


clear sound_1;

%upsampling
%
s2interp = zeros(NCell, size(s2,2)*ratio);
sound_2 = zeros(NCell,size(s2,2)*ratio,2);
for ii = 1:NCell
    fprintf('upsamping and convoluting cell %i for source 2\n', ii);
    s2interp(ii,:) = interp1(t,s2(ii,:),ti,'spline');
    % fprintf('convoluting cell %i\n', ii);
    sound_2(ii,:,1) = conv(s2interp(ii,:),HRIR(ii,:,1),'same');
    sound_2(ii,:,2) = conv(s2interp(ii,:),HRIR(ii,:,2),'same');
end

clear s2 s2interp;

toc
tic
fprintf('saving matlab workspace\n')
save('-v7.3','sound_2.mat','sound_2')

SUM_sound_2 = squeeze(sum(sound_2,1));
fprintf('writing sound\n')
audiowrite('sound_2.wav',SUM_sound_2/max(max(abs(SUM_sound_2))),44000);




audiowrite('sound.wav',(SUM_sound_2+SUM_sound_1)/max(max(abs(SUM_sound_1 + SUM_sound_2))),44000);


toc
%downsampling
% sound_down = zeros(NCell,size(s1,2),2);
% for ii = 1:NCell
%     fprintf('downsampling the convolved signal cell %i\n', ii); 
%     sound_down(ii,:,1) = interp1(ti,sound(ii,:,1),t,'spline');
%     sound_down(ii,:,2) = interp1(ti,sound(ii,:,2),t,'spline');
% end




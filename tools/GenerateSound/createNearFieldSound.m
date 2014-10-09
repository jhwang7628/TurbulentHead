function NearFieldSound 

s1 = load('../../out/source1.txt');
s2 = load('../../out/source2.txt');

HRIR_folder = '/home/jui-hsien/Research/Ansys-run/HumanHead-2/HRIR/data_head';

FileLsL = dir([HRIR_folder, 'head_left_*']);
FileLsR = dir([HRIR_folder, 'head_right_*']);


% FileStart = str2num(FileLs(1).name(10:end));
% FileEnd   = str2num(FileLs(end).name(10:end));


for ii = 1:length(FileLsL)
    ii
end



















% A = dlmread([HRIR_folder,FileLsL(1).name],'',1,0);
% 
% NumCell     = size(A,1); 
% NumDataCol  = 4;
% NumFile = length(FileLs);
% 
% Y = zeros(NumCell, NumDataCol, NumFile);
% % Y = zeros(NumCell, NumDataCol, 60);
% kk = 1;
% 
% for ii = 1:NumFile
%    if ii ~= 1
%       A = dlmread([data_folder,FileLs(ii).name],'',1,0); % Read without header
%    end
%    Y(:,1,kk) = A(:,1); % Cell number
%    Y(:,2,kk) = A(:,4); % Pressure
%    Y(:,3,kk) = A(:,9); % X-face-area
%    Y(:,4,kk) = A(:,10); % Y-face-area
% 
%    kk = kk + 1;
%    fprintf(['Loading file ', FileLs(ii).name, ' ... \n'])
% 
% end
% 
% fprintf(['Data from ', FileLs(1).name, ' to ', FileLs(NumFile).name, ' were successfully read to the system.\n']);

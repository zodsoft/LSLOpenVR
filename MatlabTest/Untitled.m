Filename = 'E:\SachsLab\Studies\DBSStudy\Recordings\TestOpenVR\untitled.xdf';
[Streams,FileHeader] = load_xdf(Filename);

stream_names = cell(1, length(Streams));
for ix = 1:length(Streams)
    stream_names{ix} = Streams{ix}.info.name;
end
pose_stream = strcmpi(stream_names, 'OpenVRPoses');
tvec = Streams{pose_stream}.time_stamps; tvec = tvec-tvec(1);
data = Streams{pose_stream}.time_series;
Fs = length(tvec)/(tvec(end) - tvec(1));

data = reshape(data, [4, 3, 3, length(tvec)]);
pos_dat= squeeze(data(4,:,:,:));

dat1 = squeeze(pos_dat(:, 1, :));
dat2 = squeeze(pos_dat(:, 2, :));
dat3 = squeeze(pos_dat(:, 3, :));

tmp1 = dat1;
tmp2 = dat2;

subplot(2,1,1)
plot(tvec, dat1)
hold on
plot(tvec, dat2)
plot(tvec, dat3)
legend('HMDX', 'HMDY', 'HMDZ', 'RtX', 'RtY', 'RtZ', 'LfX', 'LfY', 'LfZ')
xlabel('Time (s)')
ylabel('Loc. (m)')

subplot(2,1,2)
plot3(dat1(1, :), dat1(2, :), dat1(3, :))
hold on
plot3(dat2(1, :), dat2(2, :), dat2(3, :))
plot3(dat3(1, :), dat3(2, :), dat3(3, :))
legend('HMD', 'Rt', 'Lf')
xlabel('X (m)')
ylabel('Y (m)')
zlabel('Z (m)')
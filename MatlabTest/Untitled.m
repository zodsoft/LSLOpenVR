Filename = 'E:\SachsLab\Studies\DBSStudy\Recordings\TestOpenVR\untitled.xdf';
[Streams,FileHeader] = load_xdf(Filename);
tvec = Streams{2}.time_stamps;
data = Streams{2}.time_series;

tmp1 = data(4:4:12, :);
tmp2 = data(16:4:24, :);
tmp3 = data(28:4:36, :);

subplot(2,1,1)
plot(tvec, tmp1)
hold on
plot(tvec, tmp2)

subplot(2,1,2)
plot3(tmp1(1, :), tmp1(2, :), tmp1(3, :))
hold on
plot3(tmp2(1, :), tmp2(2, :), tmp2(3, :))
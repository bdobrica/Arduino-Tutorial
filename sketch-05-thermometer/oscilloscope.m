port = 'COM5';
max_range = 10;
time = (1:max_range) - 1;
vals = zeros(1,max_range);
ylim ([0 1024]);

p = plot (time, vals);
grid on;
ylim([0 1024]);
xbounds = xlim();
set (gca, 'xtick', xbounds(1):1:xbounds(2));


for i = 1:60
  val = rand();
  val = read_arduino (port, 1, 1);
  time = [ time, max(time) + 1 ];
  vals = [ vals, val ];
  view_range = (size(time,2) - max_range + 1) : (size(time,2));
  set (p, 'XData', time(view_range), 'YData', vals(view_range));
  xbounds = xlim();
  set (gca, 'xtick', xbounds(1):1:xbounds(2));
  %xbounds = xlim()
  %set (gca, 'xtick', xbounds(1):0.1:xbounds(2))
  %ylim([0 1024]);
  %pause(1);
  fflush(stdout);
end
function [u, i, pin, pout] = sps_model (f, w)

global U0;
global r;
global L;
global C;
global R;

U0 = 5;     % 5 volt
r  = 5;     % 5 ohms
L  = 1e-4;  % 100 uH
C  = 47e-6; % 47 uF
R  = 100;   % 100 ohms

ton = w / f;
toff = (1 - w) / f;

step = 500;
res = 2;

%hold on;
y0 = [0;0]; % i(0) = 0, u(0) = 0
tc = 0;
i = 0;
u = 0;

while (step > 0)
  y = lsode ('k_open_diff', y0, (t = linspace(tc,tc + toff,res)'));
  
  %plot (t, y(:,1), 'r', t, y(:,2), 'b');
  
  y0 = [y(size(y,1),1);y(size(y,1),2)];
  tc = tc + toff;
  
  i = i + mean(y(:,1)) * toff;
  u = u + mean(y(:,2)) * toff;
  
  y = lsode ('k_close_diff', y0, (t = linspace(tc,tc + ton,res)'));
  
  %plot (t, y(:,1), 'r', t, y(:,2), 'b');
  
  y0 = [y(size(y,1),1);y(size(y,1),2)];
  tc = tc + ton;

  i = i + mean(y(:,1)) * ton;
  u = u + mean(y(:,2)) * ton;

  step = step - 1;
end
%hold off;

i = i / tc;
u = u / tc;
pin = i * U0;
pout = u^2/R;
function ydot = k_close_diff (y, t)
  global U0;
  global r;
  global L;
  global C;
  global R;
  ydot(1) = (1 / L) * (U0 - y(1) * r);        % this is i()
  ydot(2) = y(2) / (R * C);                   % this is u()
end
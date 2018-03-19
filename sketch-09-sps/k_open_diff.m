function ydot = k_open_diff (y, t)
  global U0;
  global r;
  global L;
  global C;
  global R;
  if ((y(1) < 0) || (U0 - y(1) * r - y(2) < 0.5))
    y(1) = 0;
  end
  ydot(1) = (1 / L) * (U0 - y(1) * r - y(2)); % this is i()
  ydot(2) = (1 / C) * (y(1) - y(2) / R);      % this is u()
end
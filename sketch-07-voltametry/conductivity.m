## Author: bdobr <bdobr@BOGDANEL-DW10>
## Created: 2017-04-10

function retval = conductivity (pH)
  % [H+] * [OH-] = 10^-14
  % conductivity (S/m) = 0.1 * (349.82 * [H+] + 198.60 * [OH-])
  retval = 0.1 * (349.82 * 10.^(-pH) + 198.60 * 10.^(pH-14)); % S/m
  %retval = 10000 * retval; % uS/cm
endfunction
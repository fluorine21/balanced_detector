

name = "TCM1-43X+_1";
s_file = name + ".s3p";
o_file = name + ".ckt";
n_poles = 200;

S = sparameters(s_file);
fit = rationalfit(S, 'NPoles', n_poles, 'TendsToZero',false);
generateSPICE(fit, o_file);

fprintf("Done with %s\n", o_file);
clc;
clear;

file_name = '260001_GPDF.txt'

data=load(file_name);

m1 = data(:,1);
m2 = data(:,2);
v1 = data(:,3);
v2 = data(:,4);

m = [m1, m2];
v = [v1, v2];

figure(1)
scatter(m(:,1),m(:,2),10,'.','r');
title('mean');
xlabel('1st mean');
ylabel('2nd mean');
legend('260001 data set')
grid on;

figure(2)
scatter(v(:,1),v(:,2),10,'.','b');
title('variance');
xlabel('1st variance');
ylabel('2nd variance');
legend('260001 data set');
grid on;



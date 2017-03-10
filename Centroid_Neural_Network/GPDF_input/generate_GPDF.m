clc;
clear

n = 10000

figure(1)
v = [0.5, 0.5]
for x = 0.5 : 0.1 : 3
   y = x
   g = randn(n,2)*sqrt(0.1*x*x) + [x y];
   g = abs(g)
   v = [v ; g];
  
    
end

scatter(v(:,1),v(:,2),10,'.')
grid on

figure(2)
m = [0.5, 0.5]

for x = -0.5 : 0.1 : 2
   y = x*x*x*x*x + x*x*x*x + x*x*x + x*x +x +3
   y = y/40
   g = randn(n,2)*sqrt(0.1*x*x) + [x y];
   
   m = [m ; g];
  
    
end

scatter(m(:,1),m(:,2),10,'.')
grid on;

data = [m,v]

dlmwrite('260001_GPDF.txt',data,'\t');




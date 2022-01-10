clear all
clc
%% system's parameters
s =tf('s');
A1 = 5*10^(-4); %m^2
A2 = A1;
beta = 8*10^8;  %Pa
m = 20;         %kg
V1 = 10*10^(-6);%m^3   
V2 = V1;
Cq = 0.50;
a_0 = 0.05;     %mm^2
Bc = 5000;      %N/(m/s)
Kc =100000;     %N/m
mu = 0.60;      
rho = 850;      %kg/m^3 
xv0 = 0.02;     %mm
Ps = 10*10^8;   %Pa
P10 = 1*10^8;   %Pa
P20 = 1*10^8;   %Pa


%% servo valve's flow eq.
p1 = Cq*a_0*xv0/(2*sqrt(rho*(Ps - P10)));
q1 = Cq*a_0*sqrt(2/rho*(Ps - P10));

p2 = Cq*a_0*xv0/(2*sqrt(rho*(P20)));
q2 = Cq*a_0*sqrt(2/rho*(P20));


%% Rolling Stand
 
A = [-(beta*p1)/V1 0 0 -(beta*A1)/V1 0; 
     0 -(beta*p2)/V2 0 (beta*A2)/V2 0; 
     0 0 0 1 0;
     A1/m -A2/m -Kc/m -(Bc+mu)/m -1/m;
     0 0 0 0 0];
 
B = [(beta*q1)/V1 0; 
     -(beta*q2)/V2 0;
     0 0;
     0 0;
     0 1];  
  
M = [0 0 1 0 0 ;
     0 0 0 0 1 ];

C = [1 0 0 0 0 ;   
     0 1 0 0 0 ;
     0 0 1 0 0 ;
     0 0 0 1 0 ;
     0 0 0 0 1 ];
 
N = [0 0;0 0;1 1;0 0;0 0];
sys = ss(A,B,C,0);
sysd = c2d(sys,0.01);
sysc = ss(A,[B N],C,0);
sysd_2 = c2d(sysc,0.01);
%% disturbances
wt_c = tf([2],[2 1]);
wt_d = c2d(wt_c,0.01);
we_c = tf([0 20 0],[1 20 100^2]);
we_d = c2d(we_c,0.01);
%% observability//controllability
ob = obsv(sys.A , sys.C);
rank(ob)
Co = ctrb(sys.A , sys.B) ;
rank(Co,10^(-3))
%% LQ weight matrices
Q1 = [0,0,10,0,10];
Q1 = diag(Q1);
Q2 = [1,1];
Q2 = diag(Q2);
I = eye(5); 

%% LQ Controller discrete time
Ly = dlqr(sysd.A,sysd.B,Q1,Q2,0);
Lr = pinv(M*(inv(I-sysd.A+(sysd.B*Ly))*sysd.B));
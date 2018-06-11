import matplotlib.pyplot as plt
import numpy as np

#x1=Θ
#x2=Θ'
#x1'=Θ'=x2
#x2'=Θ''

#x1'=x2
#x2'=(ucos(x1)+gsin(x1))/l
#Θ''=(ucosΘ + gsinΘ)/l
l=1
g=-9.8

def f(th, w, t, ux):
	return (ux*np.cos(th)+g*np.sin(th))/l

def RungeKutta(ux, h, state):#[th, w, t]
	th=state[0]
	w=state[1]
	t=state[2]
	
	k1=h*w
	l1=h*f(th, w, t,ux)
	k2=h*(w+0.5*l1)
	l2=h*f(th+0.5*k1, w+0.5*l1, t+0.5*h,ux)
	k3=h*(w+0.5*l2)
	l3=h*f(th+0.5*k2, w+0.5*l2, t+0.5*h,ux)
	k4=h*(w+l3)
	l4=h*f(th+k3, w+l3, t+h,ux)
	
	th+=(k1+2*k2+2*k3+k4)/6
	w+=(l1+2*l2+2*l3+l4)/6
	t+=h
	return [th, w, t]

sigma=0.15
def PB(x):
	return np.exp(-((x-0.5)**2)/((2*sigma)**2)) if x<0.5 else 1

def PS(x):
	return np.exp(-((x-0.25)**2)/((2*sigma)**2))

def Z(x):
	return np.exp(-(x**2)/((2*sigma)**2))

def NS(x):
	return np.exp(-((x+0.25)**2)/((2*sigma)**2)) 

def NB(x):
	return np.exp(-((x+0.5)**2)/((2*sigma)**2)) if -0.5<x else 1

rules=[
	[PB, PB, NB],
	[PS, PS, NS],
	[Z, Z, Z],
	[NS, NS, PS],
	[NB, NB, PB],
]

def Bden(th, w, y):
	values=[]
	for rule in rules:
		Ai=rule[0](th)
		Bi=rule[1](w)
		Ci=rule[2](y)
		values.append(min(Ai, Bi, Ci))
	return max(values)

def Bnum(th, w, y):
	return y*Bden(th, w, y)

def Integrate(F, th, w, a, b, n):
	if n%2==1:
		n+=1
	h=(b-a)/n
	suma=F(th, w, a)+F(th, w, b)
	for i in range(1, n, 2):
		suma+=4*F(th, w, a+i*h)
	for i in range(2, n, 2):
		suma+=2*F(th, w, a+i*h)
	return suma*h/3

def u(th, w):
	return Integrate(Bnum, th, w, -2, 2, 70)/Integrate(Bden, th, w, -2, 2, 70)

endTime=15
step=0.1
iterations=int(15/step)
th0=np.pi/4
w0=0
t0=0
estado=[th0, w0, t0]

timeValues=[]
thetaValues=[]
omegaValues=[]
acelValues=[]
alfa1Values=[]
alfa2Values=[]

for i in range(0,iterations):
	timeValues.append(estado[2])
	thetaValues.append(estado[0])
	omegaValues.append(estado[1])
	acel=u(estado[0], estado[1])
	#print('Acel=' + str(acel))
	acelValues.append(acel)
	
	estado=RungeKutta(acel, step, estado)

plotRulesX=np.arange(-2, 2, 0.05)
plotRulesY=[]
plotRulesY.append([PB(i) for i in plotRulesX])
plotRulesY.append([PS(i) for i in plotRulesX])
plotRulesY.append([Z(i) for i in plotRulesX])
plotRulesY.append([NS(i) for i in plotRulesX])
plotRulesY.append([NB(i) for i in plotRulesX])

plt.subplot(4,1,1)
plt.xlim(-2,2)
plt.ylim(0,1.2)
plt.title("Reglas Difusas")
for i in plotRulesY:
	plt.plot(plotRulesX, i, 'r-')

plt.subplot(4,1,2)
plt.xlim(0,15)
plt.title("Angulo")
plt.plot(timeValues, thetaValues, 'b-')

plt.subplot(4,1,3)
plt.xlim(0,15)
plt.title("Velocidad Angular")
plt.plot(timeValues, omegaValues, 'b-')

#plt.subplot(5,1,4)
#plt.xlim(0,15)
#plt.title("Aceleracion Angular")
#plt.plot(timeValues, alfa1Values, 'r-')
#plt.plot(timeValues, alfa2Values, 'b-')


plt.subplot(4,1,4)
plt.xlim(0,15)
plt.title("Señal de Control: Aceleracion")
plt.plot(timeValues, acelValues, 'b-')

plt.show()
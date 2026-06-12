from pint import UnitRegistry
import numpy as np 

reg = UnitRegistry()


############## INSIRA AQUI AS DIMENSÕES FÍSICAS, POROSIDADE E VAZÃO ########################################

#14024x8136

length_p = ( 14.00 * reg("mm") ).to_base_units()
height_p = ( 8.00 * reg("mm") ).to_base_units()
porosity = 0.6819
flowrate_p = (50 * reg("uL/min") ).to_base_units()   


########## INSIRA AQUI O DELTA X - COMPRIMENTO DE 1 VOXEL ####################
deltax = 8e-6 * reg( "m" )

length_lbm = length_p / deltax
height_lbm = height_p / deltax

depth_lbm  = 32                     ## valor fixado para termos 32 camadas resolvendo escoamento
depth_p = 32 * deltax

## CALCULANDO PARÂMETROS FÍSICOS A PARTIR DOS FORNECIDOS
stension_p = ( 44e-3 * reg("N/m") ).to_base_units()
volume_p = length_p*depth_p*height_p
V_pores_phys = volume_p*porosity
Area_avg_p = depth_p * height_p * porosity 
velocity_p_avg = flowrate_p/Area_avg_p
time_p = (V_pores_phys / flowrate_p).to_base_units()
Area_avg_lbm = (depth_lbm)*(height_lbm)*porosity
Volume_lbm = length_lbm*depth_lbm*height_lbm*porosity

######## PROTOCOLO ##########
prot = 1       #### 0 PARA IMBIBIÇÃO, 1 PARA DRENAGEM - ÓLEO MOLHANTE

if (prot):
    protocol = "drainage"                                         ## this means water is the inlet fluid
    
    dviscosity_out_p = ( 1.4  * reg("mPa*s") ).to_base_units()           ## oil parameters
    density_out_p = ( 1.82 * reg( "g/cm**3" ) ).to_base_units() 
    kviscosity_out_p = dviscosity_out_p /density_out_p
    
    density_in_p =  ( 1 * reg( "g/cm**3" ) ).to_base_units()             ## water parameters
    dviscosity_in_p = 0.71 * dviscosity_out_p    
    kviscosity_in_p = (dviscosity_in_p)/density_in_p  
    
    Ca = (dviscosity_in_p*velocity_p_avg/(stension_p)).to_base_units()
    
    deltarho = 1 * reg( "g/cm**3" )                                      ## density on inlet defines lbm value
    density_lbm = (density_in_p/deltarho).to_base_units()   
    deltat = np.sqrt(deltax**3*0.06*deltarho/stension_p)
    
    dviscosity_in_lbm = dviscosity_in_p / (deltarho*deltax**2/deltat)   ## we are keeping the dynamic visc ratio! 
    dviscosity_out_lbm = dviscosity_out_p / (deltarho*deltax**2/deltat)
    kviscosity_in_lbm = dviscosity_in_lbm / density_lbm
    kviscosity_out_lbm = dviscosity_out_lbm / density_lbm
    
    kviscratio = kviscosity_in_p/kviscosity_out_p                               ## water/oil
    dviscratio = dviscosity_in_p/dviscosity_out_p
    dviscratiolbm = dviscosity_in_lbm/dviscosity_out_lbm
    kviscratiolbm = kviscosity_in_lbm/kviscosity_out_lbm
    
    tau_in_lbm = 3*kviscosity_in_lbm + 0.5
    tau_out_lbm = 3*kviscosity_out_lbm + 0.5
else:
    protocol = "imbibition"                                      ## this means oil is the inlet fluid
    
    density_in_p =  ( 1.82 * reg( "g/cm**3" ) ).to_base_units()
    dviscosity_in_p = ( 1.4  * reg("mPa*s") ).to_base_units()     
    kviscosity_in_p = (dviscosity_in_p)/density_in_p 
    
    dviscosity_out_p = dviscosity_in_p * 0.71        
    density_out_p = ( 1.00 * reg( "g/cm**3" ) ).to_base_units()
    kviscosity_out_p = dviscosity_out_p /density_out_p
    
    Ca = (dviscosity_in_p*velocity_p_avg/(stension_p)).to_base_units()
    
    deltarho = 1.82 * reg( "g/cm**3" )       
    density_lbm= (density_in_p/deltarho).to_base_units()
    deltat = np.sqrt(deltax**3*0.06*deltarho/stension_p)
    
    dviscosity_in_lbm = dviscosity_in_p / (deltarho*deltax**2/deltat)   ## we are keeping the dynamic visc ratio! 
    dviscosity_out_lbm = dviscosity_out_p / (deltarho*deltax**2/deltat)
    kviscosity_in_lbm = dviscosity_in_lbm / density_lbm
    kviscosity_out_lbm = dviscosity_out_lbm / density_lbm
    
    kviscratio = kviscosity_out_p/kviscosity_in_p   ## water/oil 
    dviscratio = dviscosity_out_p/dviscosity_in_p   ## water/oil  
    dviscratiolbm = dviscosity_out_lbm/dviscosity_in_lbm
    kviscratiolbm = kviscosity_out_lbm/kviscosity_in_lbm
    tau_in_lbm = 3*kviscosity_in_lbm + 0.5
    tau_out_lbm = 3*kviscosity_out_lbm + 0.5
    

velocity_lbm_avg = velocity_p_avg /(deltax/deltat)

stension_lbm = stension_p / (deltarho*deltax**3/deltat**2).to_base_units()   #check!
volume_lbm = porosity*length_lbm*height_lbm*depth_lbm
alpha = stension_lbm/6
tmax = (time_p/deltat).to_base_units()
Qlbm = velocity_lbm_avg * Area_avg_lbm
Catest = (kviscosity_in_lbm*density_lbm * velocity_lbm_avg / (stension_lbm)).to_base_units()

MLUPS = 120 ##### 600 EM CADA GPU - RAZOÁVEL

testimado = tmax*Volume_lbm*1e-6/2400 / 3600 * reg( "h" )     ### em horas

print(Volume_lbm)

print("Welcome to convertCC!")

print("---------------------------------------")
print("Physical quantities:")
print("Pore space of the matrix:", V_pores_phys)
print("Flow rate:",flowrate_p)
print("Time to fill the pore space:",time_p)

print("---------------------------------------")
print("Physical DIMENSIONS:")
print("Length= ", length_p.to('mm'))
print("Height= ", height_p.to('mm'))
print("Depth= ", depth_p.to('mm'))
print("---------------------------------------")

print("---------------------------------------")
print("LBM DIMENSIONS:")
print("Length= ", length_lbm)
print("Height= ", height_lbm)
print("Depth= ", depth_lbm)
print("---------------------------------------")

print("Dynamic viscosity ratio for experiment :", dviscratio )
print("Dynamic viscosity ratio for LBM (should be same as physical):", dviscratiolbm)

print("Kinectic viscosity ratio for experiment :", kviscratio )
print("Kinectic viscosity ratio for LBM (should not be same as physical):", kviscratiolbm )

print("---------------------------------------")
print("We are running:", protocol)
print("Software insertions: ")
print("TauA - Tau1 = ", tau_in_lbm)
print("TauB - Tau2 = ", tau_out_lbm)
print("Alpha = ", alpha)
print("Time-Steps= ",tmax)
print("Physical time= ",time_p)
print(f"Capillary Number =  {Catest:.5e}")
print(f"Q_LBM = {Qlbm.to_base_units()}")
print("Estimated time to run:", testimado)


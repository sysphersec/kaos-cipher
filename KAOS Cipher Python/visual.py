"""
ANIMACIÓN SIMPLE KAOS CIPHER - Atractor de Lorenz en acción

Solo necesitas: pip install matplotlib numpy
"""

import matplotlib.pyplot as plt
import numpy as np
from kaos import KaosCipher
import os

def animacion_atractor():
    # Configuración simple
    key = os.urandom(32)
    nonce = os.urandom(12)
    cipher = KaosCipher()
    
    # Estado inicial
    x, y, z = cipher.kaos_key_to_state(key, nonce)
    
    # Configurar gráfico
    fig = plt.figure(figsize=(10, 6))
    ax = fig.add_subplot(111, projection='3d')
    
    print("Generando atractor de Lorenz...")
    
    # Animación
    puntos_x, puntos_y, puntos_z = [], [], []
    
    for i in range(2000):  # Menos puntos para fluidez
        x, y, z = cipher.lorenz_step(x, y, z)
        puntos_x.append(x)
        puntos_y.append(y)
        puntos_z.append(z)
        
        # Actualizar cada 100 puntos
        if i % 100 == 0:
            ax.clear()
            ax.plot(puntos_x, puntos_y, puntos_z, 'b-', alpha=0.7, linewidth=0.8)
            ax.set_xlabel('X')
            ax.set_ylabel('Y')
            ax.set_zlabel('Z')
            ax.set_title(f'KAOS Cipher - Atractor de Lorenz (punto {i})')
            plt.pause(0.01)
    
    # Gráfico final
    ax.clear()
    ax.plot(puntos_x, puntos_y, puntos_z, 'b-', alpha=0.7, linewidth=0.8)
    ax.scatter(puntos_x[0], puntos_y[0], puntos_z[0], color='red', s=50, label='Inicio')
    ax.scatter(puntos_x[-1], puntos_y[-1], puntos_z[-1], color='green', s=50, label='Fin')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('KAOS Cipher - Atractor de Lorenz Completo')
    ax.legend()
    
    plt.show()
    print("Animación completada")
    
if __name__ == "__main__":
    # Solo necesitas: pip install matplotlib numpy
    
    # Ejecutar animación
    animacion_atractor()

#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd


# In[2]:


reader = pd.read_csv("comentarios_por_pais.csv",sep = ";")


# In[3]:


reader.head()


# In[4]:


writer = reader


# In[5]:


writer['longitud'] = writer['comentario'].apply(lambda x: len(str(x)))


# In[6]:


writer.head()


# In[7]:


import matplotlib
import numpy as np
import matplotlib.pyplot as pl


# In[8]:


#grafico de longitud por pais
#el boxplot proporciona datos estadisticos como la mediana y los valores maximo y minimo de la longitud
bp = writer.boxplot(column='longitud', by='pais', figsize=(15, 6))
pl.show()


# In[9]:


import math
def calcularEntropia(mensaje):
    mensajeList = list(mensaje)
    simbolosList = list(set(mensajeList))
    frecuencias = []
    for simbolo in simbolosList:
        cant = 0
        for simbolo2 in mensajeList:
            if simbolo == simbolo2:
                cant+=1
        frecuencias.append(float(cant) / len(mensajeList))
    entropia = 0.0
    for frec in frecuencias:
        entropia = entropia + frec * math.log(frec,2)
    entropia = -entropia
    return entropia


# In[10]:


writer['entropia'] = writer['comentario'].apply(lambda x: calcularEntropia(str(x)))


# In[11]:


writer.head()



# In[12]:


bp = writer.boxplot(column='entropia', by='pais', figsize=(15, 6))
pl.show()


# ### En base al Boxplot se puede apreciar que los mensajes de Hong Kong poseen menos entropía. 

# In[13]:


pl.hist(writer["entropia"])


# ### Sin Embargo los valores se distribuyen normalmente, por lo que se puede concluir que la entropía es similar en cada país.

# In[14]:


writer.to_csv("datos_agregados.csv")


# In[15]:


data = pd.read_csv("datos_agregados.csv")


# In[16]:


data.head()


# In[ ]:





# In[ ]:





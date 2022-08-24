# Rengin


For Realtime Rendering

SSR

![image-20220601140032473](https://github.com/Conqcd/Rengin/blob/main/images/image-20220601140032473.png)

![image-20220601141119019](https://github.com/Conqcd/Rengin/blob/main/images/image-20220601141119019.png)

Screen Space Ray Tracer For Specular Effect

shadowmap:

![image-20220601142025843](https://github.com/Conqcd/Rengin/blob/main/images/image-20220601142025843.png)

Based On PCSS
the farther distance between shadow and mary,the softer shadow is

PRT

![image-20220601142111435](https://github.com/Conqcd/Rengin/blob/main/images/image-20220601142111435.png)

![image-20220601142119924](https://github.com/Conqcd/Rengin/blob/main/images/image-20220601142119924.png)

![image-20220601142126264](https://github.com/Conqcd/Rengin/blob/main/images/image-20220601142126264.png)

Based On PreCompute Radiance Transfer Function for Realtime Rendering,which compute the Sperical harmony para.

PBR

![image-20220602193511298](https://github.com/Conqcd/Rengin/blob/main/images/image-20220602193511298.png)

![image-20220602193538586](https://github.com/Conqcd/Rengin/blob/main/images/image-20220602193538586.png)

Physical based Material,which implemented by Microface Theory.The Artifact is the light blocked by inner blockage which influence on energy loss.Thereby,I precompute the energy loss and compensate while rendering.The roughest Object benefits most.

Realtime RayTracing(3D Space)

![image-20220613194245636](https://github.com/Conqcd/Rengin/blob/main/images/image-20220613194245636.png)

![image-20220613194302486](https://github.com/Conqcd/Rengin/blob/main/images/image-20220613194302486.png)

![image-20220613194320983](https://github.com/Conqcd/Rengin/blob/main/images/image-20220613194320983.png)

![image-20220613194334562](https://github.com/Conqcd/Rengin/blob/main/images/image-20220613194334562.png)

Each frame I do is one direct lighting and one indrect lighting,but the noise is everywhere.Then I use A-Trous Wavelet Method to sample and denoise.Eventually I Acclumulate last frame`s result to simulate offline raytracing.

But each frame cost many time,so I optimize by bvh based on morton code.The result is pleasant.1 fps to 16 fps in 1070 nvidia and 4 fps to 30 fps in 2060 nvidia.

![image-20220615201747174](https://github.com/Conqcd/Rengin/blob/main/images/image-20220615201747174.png)

![image-20220615201737592](https://github.com/Conqcd/Rengin/blob/main/images/image-20220615201737592.png)
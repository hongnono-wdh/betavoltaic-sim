CompositesPartB239(2022)109952
Contents lists available at ScienceDirect
Composites Part B
journal homepage: www.elsevier.com/locate/compositesb
Betavoltaic-powered electrochemical cells using TiO nanotube arrays
2
incorporated with carbon nanotubes
Na Wanga,b, Renrong Zhenga,b, Tianxi Chia,b, Tongxin Jianga,b, Zan Dinga,b, Xin Lic,
Shichao Liud, Lifeng Zhangc,**, Haisheng Sana,b,*
aPen-Tung Sah Institute of Micro-Nano Science and Technology, Xiamen University, Xiamen, 361005, China
bShenzhen Research Institute of Xiamen University, Shenzhen, 518000, China
cChina Institute of Atomic Energy, Beijing, 102413, China
dShanghai Institute of Space Power-Sources, Shanghai, 200245, China
A R T I C L E I N F O A B S T R A C T
Keywords: Highly efficient direct energy conversion utilizing the betavoltaic effect is a promising alternative for developing
Electrochemical betavoltaics of maintenance-free and long-life self-generating power cells. This work demonstrates a betavoltaic-powered
Radioisotope electrochemical cell (BPEC) using free-standing TiO2 nanotube arrays (TNTAs) incorporated with single-wall
T
C
i
a
O
rb
2
o
n
n
a n
n
o
a
t
n
u
o
b
t
e
u
s
b es i
c
s
a
o
r
t
b
o
o
p
n
e
n
63
a
N
n
i
o
/
t
N
ub
i
e
sh
s
e
(
e
S
t
W
as
C N
th
T
e
s
c
)
a
a
th
s
o
t
d
h
e
e
a
b
s
e
w
ta
e l
a
l
n
a
o
s
d
t
e
h
,
e
a
i r
p
ra
o
d
ly
ia
io
ti
d
o
i
n
d e
s o
(
u
I (cid:0)
rc
/
e
I3 (cid:0)
.
)
F r
s
e
o
e
l
s
u
t
t
a
io
n
n
d in
a
g
s
T
th
N
e
T
e
A
l
s
e c
w
tr
e
o
r
l
e
y t
fa
e,
b r
a
i
n
ca
d
t e
a
d
r
u
a
s
d
i
i
n
o
g
-
electrochemical anodization. Monte Carlo simulations were used to investigate the interaction of the incident
beta particles with the TNTAs structure. Experimentally, the photoelectrochemical characteristics were used to
optimize the structural design of the BPECs. The optimum BPEC exhibited an energy conversion efficiency of
8.74% with an open circuit voltage of 0.26 V and a short circuit current density of 0.87 μA/cm2. The enhanced
betavoltaic effect is attributed to the highly efficient separation and transport of beta-generated carriers through
the enhanced beta-electrochemical redox reactions in the 3-D TiO2/liquid heterojunction as well as the extensive
SWCNTs conductive network.
1. Introduction engineering applications owing to their low efficiency and reliability
[2].
With the extensive application requirements for wireless sensing Betavoltaic direct energy conversion from radioactive decay (beta
microsystems and networks in remote and inaccessible regions, the de- radiation) to electricity has received more attention because it can
mand for self-generating power cells with high power density, maintain- potentially offer a higher ECE than other types of nuclear cells. Recent
free, and long-life properties has dramatically increased to meet the reviews on betavoltaic cells (BCs) were published by Olsen et al. [3],
continuous demands of low-power electronics. Nuclear cells are Prelas et al. [4], and Spencer et al. [5]. According to Olsen’s calculation
considered ideal candidates for low-power electronics, and have been model, the possible efficiency-cap of BCs is at around 35% in the limit of
intensively studied over the last few decades. Zhou et al. published an wide bandgap semiconductors (WBGSs) [3]. However, in past few years,
overarching review of nuclear battery techniques [1]. It has been well recorded ECE reported on the experimental BCs is far less than this value
known that the radioisotope thermoelectric generators (RTGs) have when using the available WBGSs (e.g., 4H–SiC and diamond) [6–9]. A
been well deployed and customized for deep space probe applications. feasible strategy for enhancing the performance of BCs is to use
However, their main characteristics, e.g., the super-high cost, large size, three-dimensional (3-D) WBGSs junction-type nanostructures instead of
and low energy conversion efficiency (ECE ≈6%), make them unsuit- 2-D planar one. 3-D nanostructured silicon-based BCs were first pro-
able for the application in low-power electronics systems. As for other posed in Sun’s work [10], in which the ECE of BCs using 3-D porous
competing types of nuclear cells, such as thermophotoelectric, direct silicon p-n diodes can be increased by approximately 10 times in com-
charge collection, and thermionics cells, there are still no actual parison with that using 2-D planar one. The 3-D nanostructures
* Corresponding author. Pen-Tung Sah Institute of Micro-Nano Science and Technology, Xiamen University, Xiamen, 361005, China.
** Corresponding author.
E-mail addresses: zhanglifeng@ciae.ac.cn (L. Zhang), sanhs@xmu.edu.cn (H. San).
https://doi.org/10.1016/j.compositesb.2022.109952
Received 29 January 2022; Received in revised form 12 April 2022; Accepted 30 April 2022
Availableonline10May2022
1359-8368/©2022ElsevierLtd.Allrightsreserved.

N. Wang et al. C o m p o s i t e s P a r t B 239(2022)109952
composed of free-standing 1-D array of crystalline semiconductors (e.g., fabricated through the electrochemical anodization technique, the outer
nanotubes, nanorods, nanowires, nanofibers, nanoneedles, etc.) can and inner diameters of the nanotubes were set to be 120 and 100 nm,
serve as preferential pathways for electron migration, which has been respectively, whereas the inner diameter from the top to the bottom is
proven to be favorable for the solar energy conversion applications [11]. linearly contracted to zero, as shown in Fig. 1(b) [25].
With the same working principle as photovoltaics, the performance of Fig. 2 shows the simulated trajectories of incident beta particles in
betavoltaics can be enhanced using 3-D nanostructured semiconductors. the TSC, TNTAs, E-TNTAs, and electrolyte. In this simulation 1000 beta
Several recent studies evaluating 3-D nanostructured BCs from optimal particles emitted from 63Ni were used. The red line represents the tra-
design and experimental respects [12–15]. There are three basic facts on jectory of the beta particles, and the yellow spots represent energy
the advantages of 3-D nanostructured BCs: 1) the filling of radioisotope deposition events. The incident depth of beta particles in TNTAs is used
material into the interspace of the 3-D nanostructure dramatically in- to obtain the optimum height of nanotubes, which can be determined
creases the collection efficiency of beta particles owing to the reduction from the energy deposition density (EDD) and energy deposition ratio
of the self-shielding effect in the isotope source; 2) the loading amount of (EDR). It can be observed from Fig. 2 (a) that TSC structure shows a
radioisotope per unit volume is increased; and 3) the junction region narrow EDD spectrum with the maximum EDD in the height of 0.9 μm
area per unit volume is extended. and around 90% of EDR within height of 2 μm. The energy loss of 10%
In our previous works, the 3-D WBGS TiO2 nanotube arrays (TNTAs) can be attributed to the back scattering of beta particles on the incident
and ZnO nanorod arrays were used to fabricate BCs using a nickel-63 surface. In contrast, the TNTAs exhibited a wide EDD spectrum with
(63Ni) sheet as the radioisotope source and electrode [16–20]. Howev- approximately 76% of EDR within depth of 10 μm (see Fig. 2(b)). While
er, the separation and transport of beta-generated carriers are insuffi- the porous structure of TNTAs brings about a strong beta-trapping effect,
cient because of the low efficiency of coupling between the radioisotope the low mass density due to the hollow structure of the nanotube enables
and the nanostructure, resulting in a high recombination of some beta particles to penetrate the TNTAs, and resulting in energy loss
beta-generated carriers and thus a low ECE. Recently, a new route was (see Fig. S1). With the filling of the liquid electrolyte into the TNTAs, the
developed using interface electrochemical engineering to enhance the equivalent density of the E-TNTAs falls between that of the TNTAs and
beta energy conversion [21,22]. The liquid electrolyte with redox spe- TSC. For the TNTAs part in E-TNTAs, a middle-wide EDD spectrum was
cies can fully contact the nanostructured anode via a redox reaction to found with the maximum EDD at a the height of 2.0 μm and 60% of EDR
transfer the beta-generated carriers. Nevertheless, there remain some within height of 9 μm (see Fig. 2(c)). At same time, the electrolyte part in
problems to be solved to achieve better performance, such as inhibition E-TNTAs only absorbs approximately 16% of all of incident energy
of the hydrogen-evolution reaction, radiolysis resistance of organic dyes, within depth of 5 μm (see Fig. 2(d)). Considering the low beta absorption
and 3-D fabrication of heterojunctions based on WBGSs. of the electrolyte, the optimum height of the TNTAs was determined to
In this work, we designed and constructed a betavoltaic-powered be 9 μm to achieve the maximum collection efficiency of beta energy.
electrochemical cell (BPEC) using 3-D nanostructured TNTAs incorpo-
rated with single-wall carbon nanotubes (SWCNTs) as the anode, a
radioisotope 63Ni sheet as the irradiation source and cathode, and a 2.2. Fabrication of energy conversion structures
(cid:0) (cid:0)
polyiodide solution containing iodide/triiodide (I /I3) redox couples as
Fig. 3(a) shows the fabrication process flow of the SWCNTs-loaded
the electrolyte. Monte Carlo simulations were used to investigate the
on TNTAs structure. First, well-aligned free-standing TNTAs were
interaction of beta particles with the 3-D TNTAs nanostructures. The
preparation and characterization of the 3-D nanostructures are pre-
sented, and the betavoltaic enhancement mechanisms of BPEC are also
analyzed.
2. Design, materials and methods
2.1. Monte Carlo simulation for beta energy deposition in nanostructures
Monte Carlo (MC) simulation software GEANT4 [23] was used to
investigate the interaction of beta particles with semiconductors. The
scattering trajectories and energy deposition of incident beta particles in
semiconductor nanostructures can be simulated using the Livermore
low-energy physics model [24]. Three structural models, namely TiO2
solid cuboid (TSC), TNTAs, and TNTAs filled with electrolyte (E-TNTAs),
were built for a comparative study, as shown in Fig. 1.
The beta particles used in this simulation were emitted from a planar
63Ni source (length ×width ×height =1000 nm ×1000 nm ×200 nm)
with the average beta-energy of 17.4 keV and the maximum beta-energy
of 66.7 keV. According to the actual structure of the TiO2 nanotubes
Fig. 2. Simulated trajectory, depth-dependent EDD and EDR of incident beta
particles in (a) the TSC, (b) the TNTAs, and (c) the TNTAs part and (d) elec-
Fig. 1. Schematic structure models of (a) TSC, (b) TNTAs, (c) E-TNTAs. trolyte part in E-TNTAs.
2

N. Wang et al. C o m p o s i t e s P a r t B 239(2022)109952
Fig. 3. (a) Fabrication process flow of SWCNTs@GTNTAs structure; (b) Actual photos of (i) PTNTAs, (ii) GTNTAs, (iii) SWCNTs@GTNTAs, (iv) BTNTAs, (v)
SWCNTs@BTNTAs samples; (c) schematic structure diagram of BPEC; (d) (i) and (ii) show the actual photos of BPEC and EUVPD, respectively.
prepared using an electrochemical anodization technique on titanium electrolyte/SWCNTs@GTNTAs/Ti structure was fabricated and resin-
(Ti) sheet [26]. TNTAs growth was performed in an electrochemical encapsulated. Fig. 3(d) shows the actual photos of BPEC and EUVPD
reaction system at room temperature using Pt as the counter electrode under test.
and a Ti sheet as the working electrode. The DC working voltage was
maintained at 50 V for 45 min. Next, the as-prepared pristine TNTAs 2.4. Material characterization and device measurements
(PTNTAs) samples were annealed in argon (Ar) ambient at 500 ◦C for 2
h. The Ar-annealed TNTAs sample presented a black color, which was The surface and cross-sectional morphologies of GTNTAs were
denoted as BTNTAs. In contrast, the TNTAs samples annealed in air at analyzed using field emission scanning electron microscopy (FESEM,
the same temperature exhibits a gray color, which is denoted as Zeiss Sigma-HD, Germany). The crystal structures were characterized by
GTNTAs. Finally, the SWCNTs were ultrasonically dispersed in alcohol X-ray diffraction (XRD, Rigaku Ultima IV, Japan). Raman scattering was
solution and then magnetically stirred for 30 min to form the SWCNTs performed using a Raman spectrometer (lDSPEC Arctic) at an excitation
solution of 0.1 mg/mL. Following above step, the SWCNTs solution was wavelength of 532 nm. ESR spectra were recorded at 143 K on a Bruker
added to the surfaces of the TNTAs samples, followed by thermal EMX-10/12 spectrometer at the X-band (frequency =9.42 GHz; power
◦
treatment in air at 60 C to enhance the adhesion between the SWCNTs = 20.06 mW). A UV/Vis/NIR spectrophotometer (Varian, Cary-5000,
and the surface of the samples. The as-prepared samples are denoted as USA) was utilized to test the UV–visible absorption spectra with the
SWCNTs@GTNTAs and SWCNTs@BTNTAs. Fig. 3(b)(i-v) shows photo- wavelength range from 300 nm to 800 nm. Electrochemical impedance
graphs of the PTNTAs, GTNTAs, SWCNTs@GTNTAs, BTNTAs, and spectroscopy (EIS) (Donghua, DH7000, China) was performed using an
SWCNTs@BTNTAs samples, respectively. electrochemical workstation. A Keithley Model 2450 source meter was
used to measured the electrical parameters of the devices in a Faraday
2.3. Preparation of electrochemical betavoltaic batteries shielding box.
Fig. 3(c) shows a schematic of the BPEC. To fabricate a sandwich- 3. Results and discussion
type BPECs, a double-sided adhesive tape was first used to enclose a
solution pool around the edge of the SWCNTs@GTNTAs/Ti structure, Fig. 4(a) shows a typical top-view FESEM image of the GTNTAs
(cid:0) (cid:0)
and then the organic polyiodide (I /I3) electrolyte was filled into the sample. It can be seen that the GTNTAs are composed of the highly or-
pool. The organic electrolyte with acetonitrile as the solvent consists of dered and compacted nanotubes with the tube-diameter of about 120
0.5 M of LiI, 0.25 M of I2, 0.5 M of 4-tert-butylpyridine and 0.6 M of a, b- nm and wall-thickness of about 10 nm. Fig. 4(b) shows a side-view
Dimethyl-3-propylimidazo-lium iodide. Next, a63Ni sheet with an ac- FESEM image of the GTNTAs. The GTNTAs are aligned perpendicular
tivity of 20 mCi (63Ni/Ni, 10 ×20 mm2, HTA, China) was used as the to the Ti substrate, with a height of approximately 9 μm and a relatively
beta source and counter electrode was attached to the top of the rough surface. The partially enlarged side-view FESEM image of the
enclosed pool. Finally, a mechanical fixture was used to clamp all the GTNTAs (Fig. 4(c)) shows that the aligned nanotubes have a close-
parts together, forming a63Ni/Ni/electrolyte/SWCNTs@GTNTAs/Ti contacted interface over the entire length, which is considered to be a
structure with an active area of 1.5 cm2. For comparison study, the 3-D conductive network. A comparative FESEM observation confirmed
BPECs based GTNTAs, BTNTAs, and SWCNTs@BTNTAs were also pre- that the annealing process had no impact on the morphology and size of
pared using the same processes. To evaluate and optimize BPEC, an the TNTAs. Fig. 4(d) shows the top-view FESEM image of the GTNTAs
electrochemical ultraviolet photodetector (EUVPD) with a glass/ITO/ incorporated with SWCNTs. It can be clearly seen that the long and
3

N. Wang et al. C o m p o s i t e s P a r t B 239(2022)109952
Fig. 4. (a) Typical top-view and (b) side-view FESEM images of GTNTAs structure; (c) partial-enlarged image of side-view FESEM images of GTNTAs; (d) Top-view
FESEM image of the SWCNTs@GTNTAs structure.
flexible SWCNTs, with diameters of approximately 100 nm, wind each cm (cid:0) 1 have very similar intensity levels. All these properties for band
other and cover the surface of TNTAs, thereby forming a conductive structure and location are consistent with the properties of metallic
network that extends in all directions to provide abundant electro- SWCNTs (m-SWCNTs) [28]. The intensity ratio IG/ID is calculated to be
chemical reaction sites that are beneficial for the rapid transport and approximately 10, implying that the SWCNTs have a highly ordered
reaction of carriers with the redox species [27]. structure and contain fewer defects and amorphous carbon.
Fig. 5(a) shows the XRD patterns of the PTNTAs, GTNTAs, BTNTAs, Fig. 6(a) shows the UV–Vis–NIR absorption spectra of the PTNTAs,
SWCNT@GTNTAs and SWCNTs@BTNTAs structures. The XRD pattern GTNTAs, BTNTAs, SWCNTs@GTNTAs, SWCNTs@BTNTAs. All the
of the PTNAs suggested the amorphous nature of the unannealed TNTAs. samples displayed a strong UV absorption and distinguishing absor-
After annealing in air, the GTNTAs samples exhibit a strong (101) bance from the Vis to NIR region. The PTNTAs and GTNTAs samples
anatase diffraction peak at 2θ =25.2◦. The BTNTAs samples have two exhibited a relatively low absorbance in the Vis-NIR region, whereas the
strong anatase diffraction peaks at 2θ (101) =25.2◦and 2θ (004) =37.6◦. BTNTAs exhibited high absorbance in the same region. With the loading
The crystallization of TiO2 can increase the mobility of carriers and thus of SWCNTs, the samples exhibited a higher absorbance than that
the conductivity [11]. As a result, the carrier recombination was without SWCNTs. FTIR measurements were used to investigate surface
reduced. The diffraction peak of SWCNTs was not observed in the chemical bonding, as shown in Fig. S2. The broad bands at around 3600
SWCNTs/BTNTAs samples because of the low amount of SWCNTs and 1600 cm (cid:0) 1 originate from the deformation vibrations of water
loaded in the TNTAs. The nature of the SWCNTs was investigated by molecules adsorbed on the sample surfaces [29]. The broad features
Raman spectroscopy, as shown in Fig. 5(b). The spectrum of observed in the region of 400–900 cm (cid:0) 1 are identified in the TiO2
SWCNTs@GTNTAs shows several main characteristic bands, a radial skeletal frequency region. Absorption bands around 480 and 842 cm (cid:0) 1
breathing mode (RBM) band at ~155 cm (cid:0) 1, a D band at ~1344 cm (cid:0) 1, were observed, corresponding to the absorption bands of Ti2O3 and the
and a second-order mode 2D band at ~2678 cm (cid:0) 1. The longitudinal anatase TiO2, respectively [30]. The ESR spectra of the GTNTAs and
tangential mode corresponding to the G + peak at ~1592 cm (cid:0) 1 and the BTNTAs samples (see Fig. S3), a distinct ESR signal at g =2.002 implies
(cid:0)
transverse tangential mode corresponding to the G peak at ~1572 the existence of oxygen vacancies (OVs) in BTNTAs structure [31],
Fig. 5. (a) XRD patterns of PTNTAs, GTNTAs, BTNTAs, SWCNT@GTNTAs and SWCNT@BTNTAs; (b) Raman spectra of GTNTAs and SWCNTs@GTNTAs.
4

N. Wang et al. C o m p o s i t e s P a r t B 239(2022)109952
Fig. 6. (a) UV–Vis–NIR absorption spectra and (b) the calculated bandgap values of PTNTAs, GTNTAs, BTNTAs, SWCNTs@GTNTAs s and SWCNTs@BTNTAs.
which is not found in the GTNTAs. The thermal treatment of TiO2 in a BPEC except for the ITO electrode instead of 63Ni electrode. Fig. 7(a)
reduced atmosphere (e.g. inert gases) results in the generation of OVs exhibits the actual photo of EUVPD under test. Without an external
owing to the escape of oxygen atom from the Ti–O bond, whereas OVs voltage bias, EUVPDs can generate a photocurrent under UV radiation. 7
cannot be formed in an oxygen-rich atmosphere (e.g., air). The OV de- (b) shows the effect of the amount of SWCNTs loaded on the BTNTAs
fects are seen as electron donors, which not only can increase the con- structure on the cyclic photoresponse of EUVPDs. It is found that the
ductivity of TiO2 but also enhance the light absorption in the Vis-NIR SWCNTs amount of 0.1 mg/mL enables the maximum photocurrent in
region due to the introduction of defect energy levels in the bandgap comparison with 0.05 and 0.2 mg/mL SWCNTs. It is suggested that the
[32,33]. The bandgaps of all samples were calculated from the main optimum amount of SWCNTs results in a maximum photocurrent to
absorption edge of the spectra (see Fig. 6(b)). It can be seen that the trade off the transmittance and conductance of the SWCNTs. Fig. 7(c)
bandgap of BTNTAs and GTNTAs structure are 2.75 eV and 3.00 eV, shows a cyclic photoresponse comparison of EUVPDs based on GTNTAs,
respectively, further confirming the effect of OVs on bandgap narrowing BTNTAs, 0.1-SWCNTs@GTNTAs and 0.1-SWCNTs@BTNTAs structures.
of TiO2. The enhanced Vis-NIR absorption through the loading of The EUVPDs based on BTNTAs exhibited an approximately 13.6%
SWCNTs on the TNTAs structure can be explained by surface plasmon higher photocurrent than those based on GTNTAs. With the loading of
resonance (SPR) in m-SWCNTs [19]. The m-SWCNTs can act as absorbers 0.1-SWCNTs on GTNTAs or BTNTAs, the photocurrents are increased
for incident Vis-NIR light due to SPR induced by the interaction between approximately 25% in comparison to the structure without SWCNTs.
light and electron plasma waves at m-SWCNTs, in which the incident The in-situ EIS spectra of EUVPDs can be used to account for the
light is converted into a surface plasmon mode [34]. different photoresponse behaviors of EUVPDs. As shown in Fig. 7(d), the
To evaluate and optimize the structure of BPEC in a cost-effective semicircle radius in the low-frequency region can be used to evaluate the
manner, the UV photoelectrochemical characteristics of EUVPDs were diffusion resistance (Rdiff) at the interface between the photoanode and
used to qualitatively simulate the beta-electrochemical performance of the electrolyte. The values of the electrical parameters of the EUVPDs
BPECs, which were fabricated using same materials and structure as can be extracted from an equivalent circuit that was used to fit EIS
Fig. 7. (a) Actual photo of EUVPD under test; (b) effect of the amount of SWCNTs loaded on BTNTAs on cyclic photoresponse; (c) cyclic photoresponse comparison
and (d) the in-situ EIS spectra of EUVPDs based on TNTAs, BTNTAs, 0.1-SWCNTs@GTNTAs and 0.1-SWCNTs@BTNTAs structures. Inset is an EIS equivalent circuit.
5

N. Wang et al. C o m p o s i t e s P a r t B 239(2022)109952
spectra, as shown in Table S1. The BTNTAs/Ti structure showed a lower of SWCNTs on TNTAs, it was observed that Isc dramatically increased
diffusion resistance (Rdiff ≈38.0 Ω) than GTNTAs/Ti structure (Rdiff ≈ and Voc also improved. The optimum BPECs based on 0.1-SWCN
98.3 Ω), indicating that the introduction of OVs increased the electrical Ts@GTNTAs structure demonstrates maximum ECE of 8.74% with Voc
conductivity of the BTNTAs structure. Meanwhile, the 0.1-SWCNTs@ ≈0.26 V, Jsc ≈0.87 μA/cm2, and Pmax ≈90 nW/cm2. Table 2 shows a
BTNTAs/Ti structures have a minimum interface resistance (Rdiff ≈ comparison of the characteristic parameters between this work and
13.9 Ω) in all EUVPDs, implying the key role of SWCNTs in enhancing other reported works.
the interface charge transport, thus accounting for the best photo- To investigate the stability of the BPECs,long-term J-T and V-T tests
response behavior. of BPEC based on the 0.1-SWCNTs@GTNTAs/Ti structure were per-
Photovoltaic tests were used to evaluate the ECE of the EUVPDs. formed. As shown in Fig. 9, the V-T curve remains basically stable with a
Fig. 8(a) and (b) show the current-voltage (I–V) and power-voltage low fluctuation of 4.5% relative to Voc during the entire test time,
(P–V) characteristics of EUVPDs based on GTNTAs, BTNTAs, whereas the J-T curve exhibits a slow increase slowly at initial 35,000 s
0.1-SWCNTs@GTNTAs and 0.1-SWCNTs@BTNTAs, respectively, under and then turn to decline, the whole fluctuation reaches 16% relative to
UV radiation of 3 mW/cm2. All devices exhibited an excellent photo- Jsc. The current rise in the initial phase could be considered a process of
conductive characteristics with a high short-circuit current (Isc) and low building a self-adaptive dynamic redox reaction. The current decline in
Voc (see Fig. 8(a)). The electrical parameters were extracted and are the later phase can be attributed to the volatilization of the organic
presented in Table S2. The BTNTAs-based device can generate a higher polyiodide electrolyte from the seal defects of the device structure.
Isc and lower Voc than the GTNTAs-based device, which can be attributed Fig. 10(a) shows a schematic working principal diagram of the BPEC
to the higher conductivity and lower bandgap of the BTNTAs structure based on the Ni/63Ni/electrolyte (I (cid:0) /I3 (cid:0) )/SWCNTs@GTNTAs/Ti struc-
when compared with the GTNTAs structure. As shown in Fig. 8(b), the ture. The carriers are driven by a potential force between the Fermi
(cid:0) (cid:0)
GTNTAs-based device has a higher ECE than BTNTAs-based device, and potential (EF) of the anode and the redox potential (ERP) of I /I3,
the loading of SWCNTs can further increase the ECE. Although defect- through which the beta-generated electrons in the conduction band (CB)
engineered BTNTAs can generate larger Isc than GTNTAs, a lower Voc of TiO2 migrate to the Ti substrate and are then transferred to the 63Ni/
makes them unsuitable for photovoltaic or betaivoltaic applications. Ni cathode from the external circuit. The beta-generated holes in the
The I–V and P–V characteristics of the betavoltaic test on BPECs are valence band (VB) of TiO2 are transferred to the SWCNTs and undergo
(cid:0)
shown in Fig. 8(c) and (d), respectively. The results of the betavoltaic an oxidation reaction with the I species at the interface of the elec-
test were in line with those of the photovoltaic test. The relatively low Isc trolyte (i.e. 3I(cid:0) + 2h+→I
3
(cid:0) ). Meanwhile, the I3 (cid:0) species diffuse to the
and high Voc are due to the low activity of 63Ni compared with the results 63Ni/Ni cathode where they carry out a reduction reaction with the
of the photovoltaic test. The electrical parameters were extracted and electrons in the cathode to regenerate the I (cid:0) redox species (i.e. I(cid:0) +
3
are shown in Table 1. It is observed that the BPEC based on the BTNTAs 2e(cid:0) →3I(cid:0) ). In this process, multiple strategies are used to enhance the
structure exhibits a poor fill factor (FF =0.25), which is far less than that betavoltaic effect. First, thermal treatment promotes the crystallization
of the BPEC based on the GTNTAs structure (FF =0.61). This can be of the TNTAs structure, improving the electrical conductivity of the
explained by the negative effect of the OVs on the Voc and FF of the structure and thus reducing the recombination of EHPs. Second, with the
BPECs. It has been reported that OVs in TiO2 induce deep-level trap filling of the electrolyte into the nanotube channel of TiO2, a large
states that can serve as electron recombination centers and pathways, specific surface area of the semiconductor-liquid (TiO2/electrolyte)
(cid:0) (cid:0)
shunting TiO2 to the electrolyte [35,36]. The BTNTAs contain more OVs heterojunction is formed because the fact that the ERP of I /I3 (0.536 V
than the GTNTAs, eventually resulting in the aggravated recombination vs. NHE) is higher than that EF of TiO2 (~0.25–0. 68 V vs. NHE). The 3-D
(cid:0)
of beta-excited electrons with I3 species in the electrolyte, and thus a heterojunction network enables an effective separation of beta-
decrease in both Voc and FF of BPECs based on BTNTAs. With the loading generated EHPs, resulting in electron transport from the nanotube
Fig. 8. I–V and P–V characteristics of (a, b) EUVPD and (c, d) BPEC based on GTNTAs, BTNTAs, 0.1-SWCNTs@GTNTAs and 0.1-SWCNTs@BTNTAs structures.
6

N. Wang et al.                                                                                                                                                                                                          C  o m   p  o s  i t e s  P  a  r t  B  239(2022)109952
Table 1
Electrical parameters of BPEC based on different anode structures.
| Anode structures of  | Electrical Parameters of BPEC  |     |     |     |     |     |     |                      |     |
| -------------------- | ------------------------------ | --- | --- | --- | --- | --- | --- | -------------------- | --- |
| BPECs                |                                |     |     |     |     |     |     | Total efficiencya η  |     |
Open-circuit voltage  Short-circuit current  Short-circuit current density Jsc  Maximum power density Pmax  Fill factor
|                     | Voc (mV)  | Isc (μA)  |     | (μA/cm2)  |     | (nW/cm2)  |     | FF  (%)     |     |
| ------------------- | --------- | --------- | --- | --------- | --- | --------- | --- | ----------- | --- |
| GTNTAs/Ti           | 202.32    | 0.45      |     | 0.30      |     | 36.79     |     | 0.61  3.57  |     |
| BTNTAs/Ti           | 156.98    | 0.69      |     | 0.46      |     | 17.88     |     | 0.25  1.74  |     |
| 0.1-SWCNTs@GTNTAs/  | 262.71    | 1.30      |     | 0.87      |     | 90.07     |     | 0.40  8.74  |     |
Ti
| 0.1-SWCNTs@BTNTAs/  | 225.08  | 1.62  |     | 1.08  |     | 63.75  |     | 0.26  6.19   |     |
| ------------------- | ------- | ----- | --- | ----- | --- | ------ | --- | ------------ | --- |
Ti
a The calculated method of ECE can be seen in supporting material.
Table 2
Comparison of characteristic parameters between the this work and reported works.
Betavoltaic structure using  Open-circuit  Short-circuit  Short-circuit current  Maximum power density  Fill factor  Total efficiency  Ref.
WBGSs  voltage Voc (V)  current Isc (nA)  density Jsc (nA/cm2)  Pmax (nW/cm2)  FF  η (%)
| 4H–SiC      | 0.72    | 41.90      |     | 16.80      |     | 6.17       |     | 0.51  6.00   | [6]   |
| ----------- | ------- | ---------- | --- | ---------- | --- | ---------- | --- | ------------ | ----- |
| 4H–SiC      | 2.09    | ~          |     | 75.47      |     | 135.65     |     | 0.86  18.60  | [8]   |
| GaN         | 1.25    | 0.17       |     | 14.54      |     | 6.00       |     | 0.33  2.78   | [37]  |
| Diamond     | 1.02    | 1.27 ×103  |     | ~          |     | 0.93 μW    |     | 0.70  1.25   | [9]   |
| BTNTAs      | 1.04    | 235.00     |     | 117.50     |     | 39.20      |     | 0.32  3.79   | [16]  |
| SWCNTs@ZnO  | 0.51    | 38.19      |     | 38.19      |     | 7.78       |     | 0.41  3.58   | [19]  |
| TiO2@N719   | 0.0292  | ~          |     | 12.75      |     | 0.0950     |     | 0.26  0.48   | [22]  |
| TiO2@Pt     | 1.00    | ~          |     | 1.75 ×105  |     | 7.50 ×104  |     | ~  53.88     | [21]  |
SWCNTs@GTNTAs  0.2627  1.30  0.87  90.07  0.26  8.74  Present
work
wall of TiO2 to the Ti substrate and the hole transport from both the
nanotube surface of TiO2 and SWCNTs to redox species (see Fig.10(b)).
Electrons and holes have different transport pathways, which are
beneficial for the suppression of carrier recombination. Finally, the
highly conductive network of SWCNTs loaded on the surface of the
GTNTAs facilitates the rapid transport of carriers and their reaction with
the redox species.
|     |     |     |     |     | 4. Conclusion  |     |     |     |     |
| --- | --- | --- | --- | --- | -------------- | --- | --- | --- | --- |
In conclusion, we demonstrated a BPEC using free-standing TNTAs
incorporated with SWCNTs as the beta-anode, a polyiodide solution as
the electrolyte, and a radioisotope 63Ni/Ni sheet as the cathode as well
as the irradiation source. Free-standing TNTAs were fabricated through
| Fig.  9. Long-term  | J-T  and  V-T  | tests  of  BPEC  based  | on  0.1-SWCNTs@  |     |      |                  |              |                          |             |
| ------------------- | -------------- | ----------------------- | ---------------- | --- | ---- | ---------------- | ------------ | ------------------------ | ----------- |
|                     |                |                         |                  |     | the  | electrochemical  | anodization  | and  thermal  treatment  | processes,  |
GTNTAs/Ti structure. Inset is the test system.
which effectively improved the electrical conductivity of TiO2. The op-
timum length of the TNTAs was determined through MC simulation. By
the UV photoresponse tests of EUVPDs, the 0.1-SWCNTs@GTNTAs
structure was regarded as the optimum design for achieving the
maximum ECE of BPEC. Experimentally, the optimum BPEC exhibits an
|     |     |     |     |     |     |     | ≈0.26 V, Jsc  | ≈0.87 μA/cm2, and Pmax  | ≈90 nW/  |
| --- | --- | --- | --- | --- | --- | --- | ------------- | ----------------------- | -------- |
ECE of 8.74% with Voc
cm2. A carrier transport model was developed to explain the mecha-
nisms of betavoltaic enhancement in BPECs. It is suggested that the 3-D
TiO2/liquid heterojunction as well as the extensive SWCNTs conductive
network facilitate the separation of beta-generated carriers and the
electrochemical redox reaction.
Author statement
Na Wang and Renrong Zheng have the same contribution in meth-
odology, investigation, and writing - original draft. Tianxi Chi per-
formed part of investigation. Tongxin Jiang and Zan Ding provided
Fig. 10. (a) Schematic diagram of energy-band and working principle of BPEC
support in modeling and simulation. Xin Li and Shichao Liu provided the
| based on Ni/63Ni/electrolyte (I | (cid:0) /I3 (cid:0) )/SWCNTs@GTNTAs/Ti structure; (b) gen- |     |     |     |     |     |     |     |     |
| ------------------------------- | ---------------------------------------------------------- | --- | --- | --- | --- | --- | --- | --- | --- |
support in experimental condition and funding acquisition. Haisheng
eration, separation and transport process of beta-generated carriers in the
San and Lifeng Zhang have the same contribution in conceptualization,
electrolyte-filled TiO2 nanotube.
methodology, writing - review & editing, project administration, fund-
ing acquisition. All authors gave final approval for publication.
7

N. Wang et al. C o m p o s i t e s P a r t B 239(2022)109952
Declaration of competing interest electrochemical reduction in organic electrolytes. ACS Applied Materials &
Interfaces 2018;10(26):22174–81.
[17] Chen CS, Wang N, Zhou P, San HS, Wang KY, Chen XY. Electrochemically reduced
The authors declare that they have no known competing financial graphene oxide on well-aligned titanium dioxide nanotube Arrays for betavoltaic
interests or personal relationships that could have appeared to influence enhancement. ACS Applied Materials & Interfaces 2016;8(37):24638–44.
the work reported in this paper. [18] Zhang Q, Wang N, Zhou P, Chen CS, San HS, Wang KY, Chen XY. A betavoltaic
microbattery using zinc oxide nanowires under build in potential difference. 29th
International Conference on Micro Electro Mechanical Systems (MEMS). IEEE;
Acknowledgment 2016.
[19] Chen CS, Chen J, Wang Z, Zhang J, San HS, Liu SC, Wu CY, Hofmann W. Free-
standing ZnO nanorod arrays modified with single-walled carbon nanotubes for
This work was supported by the National Natural Science Foundation betavoltaics and photovoltaics. Journal of Materials Science & Technology 2020;
of China, China (Grant No.12175190) and Special Funds for Central 19:48–57.
Government Guiding Shenzhen Development in Science and Technol- [20] Wang N, Ma Y, Chen J, Chen CS, San HS, Cheng ZD. Defect-induced betavoltaic
enhancement in black titania nanotube arrays. Nanoscale 2018;10(27):13028–36.
ogy, China (Grant No.2021Szvup066).
[21] Kim BH, Kwon JW. Plasmon-assisted radiolytic energy conversion in aqueous
solutions. Scientific Reports 2014;4:1–9.
Appendix A. Supplementary data [22] Hwang YJ, Park YH, Kim HS, Kim DH, Ali S, Sorcar S, Flores MC, Hoffmann MR,
In S. C-14 powered dye-sensitized betavoltaic cells. Chemical Communications
2020;56(52):7080–3.
Supplementary data to this article can be found online at https://doi. [23] Agostinelli S, Allison J, Amako K, Apostolakis J, Araujo H, Arce P, et al. GEANT4 -
org/10.1016/j.compositesb.2022.109952. a simulation toolkit. Nuclear Instruments and Methods in Physics Research Section
A: Accelerators, Spectrometers, Detectors and Associated Equipment 2003;50:
250–303.
References [24] Toshito T, Bagulya AV, Lechner A, Ivanchenko V, Maaire M, Akagi T, Yamashita T.
Validation of new Geant4 electromagnetic physics models for ion therapy
[1] Zhou Y, Zhang SX, Li GP. A review of radioisotope batteries. Chinese Science applications. Progress in Nuclear Science and Technology 2011;2:918–22.
Bulletin 2017;62(17):1831–45. [25] So S, Hwang I, Yoo JE, Mohajernia S, Mackovic M, Spiecker E, Cha G, Mazare A,
[2] Steinfelds E, Prelas M, Loyalka SK, Tompson RV. A comparison of the performance Schmuki P. Inducing a nanotwinned grain structure within the TiO2 nanotubes
capabilities of radioisotope energy conversion systems, beta voltaic cells, and other provides enhanced electron transport and DSSC efficiencies >10%. Advanced
nuclear batteries. 2006. p. 2696–706. Proceedings of the 2006 International Energy Materials 2018;8(33):1800981.
Congress on Advances in Nuclear Power Plants, ICAPP. [26] Grimes CA. Synthesis and application of highly ordered arrays of TiO2 nanotubes.
[3] Olsen LC, Cabauy P, Elkind BJ. Betavoltaic power sources. PhysicsToday 2012;65 Journal of Materials Chemistry 2007;17(7):1451–7.
(12):35–8. [27] Pan X, Zhu K, Ren GF, Islam N, Warzywoda J, Fan ZY. Electrocatalytic properties of
[4] Prelas MA, Weaver CL, Watermann ML, Lukosi ED, Schott RJ, Wisniewski DA. a vertically oriented graphene film and its application as a catalytic counter
A review of nuclear batteries. Progress Nuclear Energy 2014;75:117–48. electrode for dye-sensitized solar cells. Journal of Materials Chemistry A 2014;2(3):
[5] Spencer MG, Alam T. High power direct energy conversion by nuclear batteries. 12746–53.
Applied Physics Reviews 2019;6(3):031305. [28] Chen CS, Zhang SD, Hu BF, San HS, Cheng ZD, Hoffmann W. Non-aligned ZnO
[6] Chandrashekhar MVS, Thomas CI, Li H, Spencer MG, Lai A. Demonstration of a 4H nanowires composited with reduced graphene oxide and single-walled carbon
SiC betavoltaic cell. Applied Physics Letters 2006;88(3). 033506 - 033506-3. nanotubes for highly responsive UV–visible photodetectors. Composites Part B:
[7] Eiting CJ, Krishnamoorthy V, Rodgers S, George T, Robertson JD, Brockman J. Engineering 2019;164:640–7.
Demonstration of a radiation resistant, high efficiency SiC betavoltaic. Applied [29] Zhao JY, Yao JX, Zhang YZ, Guli M, Xiao L. Effect of thermal treatment on TiO2
Physics Letters 2006;88:064101. nanorod electrodes prepared by the solvothermal method for dye-sensitized solar
[8] Thomas C, Portnoff S, Spencer MG. High efficiency 4H-SiC betavoltaic power cells: surface reconfiguration and improved electron transport. Journal of Power
sources using tritium radioisotopes. Applied Physics Letters 2016;108:013505. Sources 2014;255:16–23.
[9] Bormashov VS, Troschiev SY, Tarelkin SA, Volkov AP, Teteruk DV, Golovanov AV, [30] Kumar PM, Badrinarayanan S, Sastry M. Nanocrystalline TiO2 studied by optical,
Kuznetsov MS, Kornilov NV, Terentiev AS, Blank VS. High power density nuclear FTIR and X-ray photoelectron spectroscopy: correlation to presence of surface
battery prototype based on diamond Schottky diodes. Diamond and Related states. Thin Solid Films 2000;358(1–2):122–30.
Materials 2018;84:41–7. [31] Bad’ura Z, Naldoni A, Qin SS, Bakandritsos A, Kment S ˇ , Schmuki P, Zoppellaro G.
[10] Sun W, Kherani NP, Hirschman KD, Gadeken LL, Fauchet PM. A three-dimensional Light-induced migration of spin defects in TiO2 nanosystems and their contribution
porous silicon p-n diode for betavoltaics and photovoltaics. Advanced Materials to the H2 evolution catalysis from water. ChemSusChem 2021;14(20):4408–14.
2005;10:1230–3. [32] Wu H, Irani R, Zhang KF, Jing L, Dai HX, Chung HY, Abdi FF, Ng YH. Unveiling
[11] Wu H, Tan HL, Toe CY, Scott J, Wang LZ, Amal R, Ng YH. Photocatalytic and carrier dynamics in periodic porous BiVO4 photocatalyst for enhanced solar water
photoelectrochemical systems: similarities and differences. Advanced Materials splitting. ACS Energy Letters 2021;6(10):3400–7.
2020;32(18):1904717. [33] Pan XY, Yang MQ, Fu XZ, Zhang N, Xu YJ. Defective TiO2 with oxygen vacancies:
[12] Duggirala R, Tin S, Lal A. 3D silicon betavoltaics microfabricated using a self- synthesis, properties and photocatalytic applications. Nanoscale 2013;5(9):
aligned process for 5 milliwatt/cc average, 5 year lifetime microbatteries. 3601–14.
International Solid-State Sensors, Actuators and Microsystems Conference. IEEE; [34] Park YR, Liu N, Lee CJ. Photoluminescence enhancement from hybrid structures of
2007. p. 279–82. metallic single-walled carbon nanotube/ZnO films. Current Applied Physics 2013;
[13] Guo H, Yang H, Zhang Y. Betavoltaic microbatteries using porous silicon. 20th 13(9):2026–32.
International Conference on Micro Electro Mechanical Systems (MEMS). IEEE; [35] Rajaraman TS, Parikh SP, Gandhi VG. Black TiO2: a review of its properties and
2007. p. 867–70. conflicting trends. Chemical Engineering Journal 2020;389:123918.
[14] Murphy JW, Voss LF, Frye CD, Shao Q, Kazkaz K, Stoyer MA, Henderson RA, [36] Shrivastava N, Barbosa H, Ali K, Sharma SK. Materials for solar cell applications: an
Nikolic RJ. Design considerations for three-dimensional betavoltaics. AIP Advances overview of TiO2, ZnO, upconverting organic and polymer-based solar cells. Solar
2019;9(6):065208. Cells 2020:55–78.
[15] Krasnov A, Legotin SA. Advances in the development of betavoltaic power sources [37] Xi SX, Li LX, Zhou CZ, Li HJ, Huang GW, Wu K, Wang ZG, Zhang YY. Researches on
(A review). Instruments and Experimental Techniques 2020;63(4):437–52. the performance of GaN-PIN betavoltaic nuclear battery. Radiation Effects and
[16] Ma Y, Wang N, Chen J, Chen CS, San HS, Chen JG, San HS, Cheng ZD. Betavoltaic Defects in Solids 2022:1–7.
enhancement using defect-engineered TiO2 nanotube arrays through
8
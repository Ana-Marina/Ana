Bulea Ana-Marina
Tema EGC 2

Jucatorul se controleaza din tastele a, d (miscare pe orizontala) si
space(salt). Jucatorul si camera se pot reseta cu tasta c. Proiectia
din viewportul secundar se poate scimba din tastele o (ortogonala)
si p (perspectiva). Viewportul primar foloseste camera TPS iar
viewportul secundar camera FPS. FoV-ul proiectiei perspectiva se poate
modifica cu tastele + si -.

Scena este alcatuita din cuburi iar coliziunile cub-jucator se verifica
cu functia collision(lab::Mesh *cube) care compara distantele intre
centrele jucatorului si cubului cu lungimile laturilor. Daca pe toate
axele distanta intre centre este mai mica decat lungimea laturii cubului
 + lungimea laturii jucatorului atunci are loc o coliziune.
 
Cand jucatorul este in miscare, cadere sau salt se verifica daca miscarea
pentru frameTime-ul curent duce la o coliziune. Daca da atunci miscarea
inceteaza. Miscarea pe verticala (cadere/salt) este tratata separat
de miscarea pe orizontala deoarece o coliziune in timp ce obiectul este
in cadere duce la incetarea caderii, insa jucatorul se poate lovi de un
perete miscandu-se pe orinzontala in timp ce e in cadere. Acest caz nu
inceteaza caderea pe verticala ci doar pe orizontala.
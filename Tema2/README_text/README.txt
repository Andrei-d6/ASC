Dugăeșescu Andrei 331CC

Tema 2 ASC - Code Optimization



Varianta blas: solver_blas.c
	
	Implementarea acestei variante este cea mai scurtă din punct de vedere al codului folosit (dat fiind faptul că acesta se folosește de funcții deja existente) însa se dovedește a fi deosebit de performantă din punct de vedere al timpului de execuție. În ceea ce privește implementarea, aceasta presupune calculul a 3 înmultiri și efectuarea unei adunări "de mână". În cadrul soluției propuse sunt folosite 3 apeluri ale funcției cblas_dtrmm (funcție responsabilă pentru calculul produsului dintre o matrice superior triunghiulară și o matrice normală) precum și 3 variabile auxiliare: C - rezultatul final, valoarea întoarsă de funcție; A_2 - matrice în care este stocat rezultatul înmulțirii A*A si A_2B - matrice folosită pentru reținerea rezultatului calculului A_2 * B. Înainte de a calcula efectiv produsele necesare obținerii rezultatului final, matricile A_2 și A_2B sunt inițializate astfel: A_2 = A, A_2B = B.
	Primul apel al funcției cblas_dtrmm calculează în interiorul lui A_2 produsul A*A: A_2 *= A, iar A_2 = A. Astfel în A_2 se va afla valoarea lui A^2. Al doilea apel al funcției cblas_dtrmm este responsabil pentru calculul lui A_2B = A^2 * B. Inițializarea lui A_2B cu B a fost făcuta tocmai pentru a se putea calcula A^2*B ținând cont de faptul că A^2 este de asemenea o matrice superior triunghiulară, practic A_2B = A_2 * A_2B, iar A_2B = B la inceput. Astfel, în urma apelului, A_2B va avea valoarea lui A_2 * B. Ultimul apel este responsabil pentru calcului lui B*At. Din cauza faptului că această funcție schimbă valoarea unuia dintre parametrii de intrare (a unei matrici cu ajutorul căreia se calculează produsul) acest apel este ultimul pentru că rezultatul va modifica valoarea lui B. Cu alte cuvinte, acest ultim apel efectueaza B = B * At.
	După ce au fost calculate toate înmulțirile de matrici, se adună rezultatele obținute și sunt salvate în cadrul matricei C, matrice care va reprezenta rezultatul final.


Varianta neoptimizată: solver_neopt.c

	Pentru această variantă fără îmbunătățiri - "de mână", s-au folosit 3 "grupări" de for-uri unde este realizat calculul efectiv. Prima grupare (primele 3 for-uri) sunt responsabile pentru calculul matricei rezultate din A*A, rezultat stocat în interiorul matricei A_2 (nume provenit de la A^2). Pentru calculul lui A*A este parcursă partea superioară diagonalei principale a matricei A, deoarece A este o matrice superior triunghiulară iar pentru calculul efectiv al unei valori din A_2 se parcurg elementele din A care se știu că sunt nenule, adică până în diagonala principală. Cu alte cuvinte, pentru fiecare element ce se dorește a fi calculat din matricea rezultat A_2 sunt folosite doar acele valori aflate în partea nenulă a matricei A (din partea care conține numere diferite de 0). A doua grupare de for-uri este responsabilă pentru calculul (A*A)*B (A_2 * B). Rezultatul acestui calcul este păstrat în cadrul matricei C pentru ca restul calculelor să fie adunate direct în C pentru a avea rezultatul final direct în valoarea întoarsă de funcție - C (se evita astfel reținerea rezultatului A_2 * B într-o matrice care se va aduna la matricea C). Dacă în cazul lui A*A rezultatul era de asemena o matrice superior triunghiulară, în cazul A_2 * B rezultatul nu mai este nepărat tot o matrice superior triunghiulară (dacă B este o matrice normală atunci rezultatul va reprezenta tot o matrice normală). Cu toate acestea, nu este nevoie să luam în calcul valorile de 0 cunoscute din interiorul matricei A_2. Ultima grupare de for-uri (a treia) este responsabilă pentru calculul B*At (At - transpusa lui A). Calculul respectiv este adunat la valorile deja calculate din C pentru a obține rezultatul final. În timpul înmulțirii acestor două ultime matrici: B și At putem ține cont de faptul că transpusa unei matrici superior triunghiulare este o matrice inferior triunghiulară, evitând astfel includerea 0-urilor cunoscute în calculul rezultatului final. Odată cu terminearea ultimei grupări de for-uri rezultatul expresiei B*At + A^2 + B se găsește în matricea C.

	
Varianta optimizată a versiunii neoptimizate (solver_neopt.c): solver_opt.c

	Această implementare are la bază funcționalitatea descrisă mai sus în cadrul versiunii solver_neopt. Pentru a obține îmbunătățiri în ceea ce priveste performanțele, în principal scăderea timpului de execuție, au fost folosite o serie de optimizări. După cum a fost menționat și mai sus, din punct de vedere structural implementarea este împărțită în 3 "grupări" de for-uri, fiecare responsabilă pentru calculul unei înmulțiri de matrici. Ordinea și funcționalitatea acestora este identică variantei neoptimizate. Pentru prima grupare, cea responsabilă pentru calculul lui A*A, s-a optat în primul rând pentru o rearanjare/reordonare a buclelor pentru creșterea performanțelor. În cadrul acestei grupări ordinea folosită este i-k-j. Pe lânga aceasta, în locul folosrii accesului clasic la memoria matricilor, această implementare se folosește de pointeri pentru a reduce numărul de operații necesare pentru accesul la elementele matricei; de asemenea, pointerii folosiți cel mai frecvent sunt precedați de cuvantul cheie register, pentru a eficientiza cât mai mult operațiile cu aceștia. Cum proprietățile matricilor nu s-au schimbat, în continuare se evită lucrul cu elementele de sub diagonala principală (întrucat este cunoscut faptul că valorile acestora sunt 0).
	Pentru cea de a doua grupare, responsabilă pentru calculul A_2 * B, se aplică aceeași strategie cu cea menționată mai sus - reordonarea buclelor din i-j-k în i-k-j, folosirea de pointeri pentru a eficientiza accesul la memorie și evitarea calculului pentru valorile care se cunosc a fi 0. În ceea ce privește ultima grupare, cea în care se calculeaza B*At, aceasta prezintă ordinea clasică a buclelor (i-k-j) din cauza unei dependințe a indicilor. Pe lângă pointerii folosiți în acest caz, dată fiind structura buclelor, este folosită o variabila (suma) pentru a calcula valoarea finală ce va trebui adăugată în rezultat, limitând astfel pe cât posibil accesul la memorie. O altă optimizare valabilă pentru toate cele 3 grupări este reprezentată de folosirea tehnicii de loop unrolling pentru bucla cea mai interioară pentru a efectua în total un număr semnificativ mai mic de iteratii, fiecare iterație însâ având totuși o serie mai mare de instrucțiuni de executat, însă per total se efectuează mai puține verificări de control a buclei, se elimină din penalitățile de branch opținându-se un trade-off între dimensiunea codului - prin introducerea de linii adiționale de cod, în acest caz 10 pentru fiecare grupare dat fiind unroll-ul de 10, și timpul de execuție.
	Fiecare dintre optimizările mai sus menționate au contribuit în ceea ce privește reducerea timpului de execuție însa nu în ponderi egale. Folosirea de pointeri a redus substantial timpul de executie (aproximativ 10s vs 5s - timpi locali), iar loop unrolling-ul a redus timpul de execuție suficient de mult cât aproape să îl înjumătățească (beneficiile au fost vizibile de la un unroll de 4 la un unroll de 10).



Varianta cu optimizare cu ajutorul flag-urilor de compilare: opt_f_extra


	Această variantă de optimizare are la bază codul descris în cadrul sursei solver_neopt.c a cărui funcționalitate a fost descrisă într-un paragraf anterior. Optimizarea în sine constă în folosirea unor flag-uri de compilare care vizează reducerea timpului de execuție. În această secțiune se va discuta despre flag-urile folosite care nu sunt deja incluse in -O3, adică: -mtune=native -funroll-loops -fno-math-errno -funsafe-math-optimizations -ffinite-math-only și -fexcess-precision=fast.


-mtune=native: folosirea acestui flag provine de la observarea îmbunătățirii semnificative a performanței prin 			intermediul folosirii unui alt flag, care îl include pe acesta și anume: -march=native. -march=cpu-type este un flag responsabil pentru generarea de instrucțiuni specifice tipului de procesor specificat (native presupune selectarea procesorului întâlnit pe mașina pe care se realizeaza compilarea). Intuitiv, specializarea codului pentru mașina pe care va rula pare o idee bună iar rezultatele confirmă acest lucru. Din cauza faptului că -march va conduce la generarea unui cod specific pentru procesorul cu ajutorul căruia a fost compilat codul, acest lucru însemna faptul că odată compilat pe un anumit tip de procesor executabilul obținut în final nu va rula pe altă mașină dacă aceasta nu dispune de acelasi tip de procesor. Din aceste considerente s-a optat pentru folosirea -mtune, flag care optimizează codul generat, este mai puțin agresiv din punct de vedere al specializarii codului pentru un anumit procesor, iar performantele obținute cu ajutorul acestui flag par să nu se fi diminuat față de folosirea -march. Cu toate acestea, este indicat ca înainte de rularea executabilului pentru a observa timpii de rulare, codul să fie compilat pe mașina de testare, în cazul acestei teme, coada nehalem. Pentru facilitarea acestui lucru în cadrul arhivei este regăsit un script de testare care va compila codul pe coadă și va genera câte un fișier de output pentru fiecare variantă, fișier care va conține timpii obținuți de varianta respectivă. Pentru folosirea acestui script se dorește folosirea urmatoarei comenzi:
	//////////////////////////////////////////////////////////////
	/////////////////////// RULARE SCRIPT ////////////////////////
	//////////////////////////////////////////////////////////////
		qsub -cwd -q ibm-nehalem.q -b n check.sh
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	///////////////////// RULAREA UNEI VARIANTE //////////////////
	//////////////////////////////////////////////////////////////
		qsub -cwd -q ibm-nehalem.q -b y ./exec input
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	Fișierele generate se vor numi: blas.txt, neopt.txt, opt_m.txt, opt_f.txt și opt_f_extra.txt.


-funroll-loops: dat fiind faptul că soluția în sine constă într-o serie de loop-uri ale căror capete (indici de start 			și de sfârșit) variază, un flag care vizează o tehnica de optimizare care s-a dovedit deosebit de 	eficientă în cadrul versiunii optimizate "de mână" (solver_opt.c) părea o alegere naturală. Îmbunătățirea în ceea ce privește performanța a fost totuși minoră - deși timpul de execuție a scăzut, speed up-ul a fost marginal.

	Următoarele flag-uri fac parte dintr-o colecție de flag-uri prin intermediul căreia au fost identificate. Flag-ul -ffast-math conține restul de flag-uri folosite dar mai activează și alte flag-uri decât cele menționate.

-fno-math-errno: acest flag, conform documentației, ar trebui să conducă la o creștere a vitezei de rulare dat fiind 			faptul că dezactivează flag-ul errno pentru anumite instrucțiuni matematice. Tot în cadrul documentației găsite pentru acest flag se sugerează faptul că pentru un cod care nu necesită folosirea exactă a implementării IEEE, folosirea acestui flag ar trebui să îmbunătățească performanțele.  


-funsafe-math-optimizations: acest flag s-a dovedit a fi unul important pentru reducerea timpului de execuție. Acest 			flag permite folosirea unor anumite optimizări pentru operațiile în virgulă mobilă presupunând faptul că argumentele (valorile din cadrul operațiilor în virgulă mobilă) sunt valide (de unde și numele de unsafe). Presupunând totuși câ în cadrul temei sunt generate matrici valide din punct de vedere al conținutului (presupunem ca matricile contin doar numere) acest flag aduce îmbunătățiri vizible în timpul de execuție.

-ffinite-math-only: asemănător flag-ului de mai sus, acesta permite optimizări pentru operațiile în virgulă mobilă (în cazul acesta calculul valorilor matricelor) presupunând faptul că valorile cu care se lucreaza (valorile din matrici) nu sunt NaN sau Inf (Not a Number sau infinit - numarul invalid obtinut spre exemplu în urma unei operații de împărțire la 0). Cum presupunerea faptului ca matricile conțin valori valide pentru tipul de date folosit (double) nu pare a fi una foarte ambițioasă s-a optat pentru folosirea acestui flag.

-fexcess-precision=fast: aceasta optiune permite o formă de control asupra preciziei penrtu operațiile în virgulă mobilă. Folosirea acestei optiuni (fast) înseamnă că operațiile pot fi efectuate cu o precizie mai largă decât tipurile specificate; acest lucru poate conduce la un cod mai rapid însa mai puțin previzibil din punct de vedere al previziblității rotunjirilor la tipul specificat. Din punct de vedere al preformanțelor acest flag este de ajutor însă nu are un impact la fel de mare precum -mtune sau -funsafe-math-optimization.

	În concluzie, pentru obținerea unui speed-up vizibil față de folosirea doar a opțiunii -O3, cele mai importante flag-uri folosite sunt reprezentate de -mtune=native și -funsafe-math-optimizations. Prin presupunerea validității datelor (o presupunere care nu ar trebui sa fie una foarte îndrăzneață) și prin optimizarea instrucțiunilor pentru codul oferit s-a putut atinge un timp de executie mai rapid (din testele efectuate pe coadă cu aproximativ 11-14% mai rapid).

	






	





































	 

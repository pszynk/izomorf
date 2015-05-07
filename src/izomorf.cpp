/**
 *      @file  izomorf.cpp
 *     @brief  plik z metodą main
 *
 * Detailed description starts here.
 *
 *=====================================================================================
 */


#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <iterator>
#include <chrono>
#include <exception>

//#define NDEBUG
#include <assert.h>
#include <vector>

#include "graph.hpp"
#include "isomorphismAlgo.hpp"

#define VERTEX_SIZE_PRINT 10

using namespace std;

/**
 * @brief zwraca wiadomość pomocniczą programu
 *
 * @return string z wiadomością pomocniczą.
 */
std::string helpMsg()
{

    stringstream ss;

    ss  << "Program do weryfikowania izomorfizmu grafów" << endl
        << "IZOMORF [OPCJA] [VAL1] [VAL2]" << endl
        << " OPCJE:" << endl
        << string(80, '-') << endl
        << "    f <plik z grafem 1> <plik z grafem 2> " << endl
        << "          wczytaj grafy z plików i przetestuj ich izomorfizm" << endl
        << string(80, '-') << endl
        << "    r <V = liczba wierzchołków> <D = gęstość> " << endl
        << "          wygeneruj graf dwa izomorficzne grafy losowe o danej ilości" << endl
        << "          wierzchołków i gęstości i przetestuj ich izomorfizm" << endl
        << "          0 <= V <= 1000, D in (0, 1]" << endl
        << string(80, '-') << endl
        << "    t " << endl
        << "          przeprowadź serię testów" << endl
        ;
    return ss.str();
}

/**
 * @brief wczytuje z pliku, ew. wypisuje komunikat błędu
 *
 * @param g wczytywany graf
 * @param filename nazwa pliku z grafem
 * @param graphname nazwa grafu dla komunikatu o błędzie
 */
void readGraph(Graph &g, std::string filename, std::string graphname)
{

    try {
        g.loadFromFile(filename);
    } catch (exception e) {
        cout << "Nie udało się wczytać grafu " << graphname << endl
            << "   " << e.what() << endl;
        throw e;
    }
}

/**
 * @brief wypisuje na konsolę proces weryfikacji izomorfizmu
 * dwóch grafów
 *
 * @param gX graf pierwszy
 * @param gY graf drugi
 */
void checkIsomorphism(const Graph &gX, const Graph &gY)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::duration_cast;

    IsomorphismAlgo isoalg(gX, gY);

    cout << "*** Sprawdzanie izomorfizmu grafów ***" << endl;
    cout << string(100, '=') << endl;
    cout << "Graf X:" << endl;
    cout << "    Liczba wierzchołków: " << gX.getVertexCount() << endl;
    cout << "    Liczba krawędzi:     " << gX.getEdgeCount() << endl;
    cout << "Graf Y:" << endl;
    cout << "    Liczba wierzchołków: " << gY.getVertexCount() << endl;
    cout << "    Liczba krawędzi:     " << gY.getEdgeCount() << endl;
    cout << string(100, '=') << endl;
    cout << "Czy grafy spełniają warunki wstępne..." << endl;

    if (!isoalg.meetsRequirements()) {
        cout << "NIE: warunki nie zostały spełnione" << endl;
        cout << "__NIE: grafy NIE są IZOMORFICZNE__" << endl;
        return;
    }

    cout << "TAK: spełniają warunki wstępne" << endl;
    cout << string(100, '=') << endl;
    cout << "Czy grafy są izomorficzne..." << endl;

    high_resolution_clock::time_point start = high_resolution_clock::now();
    bool flag = isoalg.isIsomorphism();
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end - start);

    if (flag) {
        cout << "__TAK: grafy są IZOMORFICZNE__" << endl;

        if (gX.getVertexCount() <= VERTEX_SIZE_PRINT) {
            cout << "Graf X: ---------------------------" << endl;
            cout << gX << endl;
            cout << "Graf Y: ---------------------------" << endl;
            cout << gY << endl;

            IsomorphismAlgo::iso_map im;
            im = isoalg.getIsoMap();
            cout << "IZOMORFIZM: -----------------------" << endl;
            for (auto iz : im) {
                cout << iz.first << " -> " << iz.second << endl;
            }

        }
    } else {
        cout << "__NIE: grafy NIE są IZOMORFICZNE__" << endl;
    }
    cout << string(100, '=') << endl;
    cout << "Czas obliczeń:" << endl;
    cout << "   " << time_span.count() << " sekund" << endl;
}

/**
 * @brief uruchom unittest
 *
 * Unittest sprawdza czy grafy spełniają warunki izomorfizmu
 * i czy są izomorficzny
 *
 * @param gX graf pierwszy
 * @param gY graf drugi
 * @param meets czy grafy powinny spełniać warunki
 * @param izom czy grafy powinny być izomorficzne
 * @param nr numer testu
 * @param testname nazwa testu
 *
 * @return czy test wykonał się poprawnie
 */
bool runTestUnit(const Graph &gX, const Graph &gY,
        bool meets, bool izom, unsigned int nr, std::string testname)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::duration_cast;

    bool retrn = false;

    cout << string(80, '*') << endl;
    cout << "WYKONYWANIE TESTU: [" << nr << "] ... |V| = "
        << gX.getSize() << "  |E| = " << gX.getEdgeCount() << endl;
    cout << " -- " << testname << endl;
    cout << string(80, '=') << endl;

    IsomorphismAlgo isoalg(gX, gY);

    duration<double> time_span;

    if (! meets) {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        retrn = (meets == isoalg.meetsRequirements());
        high_resolution_clock::time_point end = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(end - start);
    }
    else if (! izom) {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        bool flag = isoalg.meetsRequirements();
        high_resolution_clock::time_point end = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(end - start);

        if (flag) {
            start = high_resolution_clock::now();
            retrn = (izom == isoalg.isIsomorphism());
            end = high_resolution_clock::now();
            time_span = duration_cast<duration<double>>(end - start);
        } else {
            retrn = false;
        }
    }
    else {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        retrn = (izom == isoalg.isIsomorphism());
        high_resolution_clock::time_point end = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(end - start);
    }

    cout << "TEST: ";
    if (retrn) {
        cout << "OK";
    } else {
        cout << "FAIL";
    }
    cout << "    [" << time_span.count() << " s.]" << endl << endl;

    return retrn;
}

/**
 * @brief uruchom unittest na grafach zapisanych w plikach
 *
 * @param filenameX plik z grafem 1
 * @param filenameY plik z grafem 2
 * @param meets czy grafy powinny spełniać warunki izomorfizmu
 * @param izom czy grafy powinny być izomorficzne
 * @param nr numer testu
 * @param testname nazwa testu
 *
 * @return czy test wykonał się poprawnie
 */
bool runFileTest(std::string filenameX, std::string filenameY,
        bool meets, bool izom, unsigned int nr, std::string testname)
{
    try {
        Graph gX, gY;

        readGraph(gX, filenameX, "Graf X");
        readGraph(gY, filenameY, "Graf Y");

        return runTestUnit(gX, gY, meets, izom, nr, testname);
    } catch (exception e) {
        cout << "BŁĄD: złe dane do wczytywania grafów" << endl;
        cout << "TEST: FAIL" << cout;
    }
    return false;
}


/**
 * @brief uruchom unittest na grafach losowych
 *
 * Generuje dwa losowe grafy izomorficzne i weryfikuje ich
 * izomorfizm
 *
 * @param v ilosc wierzchołków w grafach losowych
 * @param d gęstość krawędzi w grafach losowych
 * @param izom czy grafy powinny być izomorficzne
 * @param nr numer testu
 * @param testname nazwa testu
 *
 * @return czy test wykonał się poprawnie
 */
bool runRandomTest(unsigned int v, double d,
        bool izom, unsigned int nr, std::string testname)
{
    try {
        Graph gX = Graph::generateRandom(v, d);
        Graph gY; gY.randomIsomorphic(gX);

        return runTestUnit(gX, gY, true, izom, nr, testname);
    } catch (exception e) {
        cout << "BŁĄD: złe dane do generowania grafu losowego" << endl;
        cout << "TEST: FAIL" << cout;
    }
    return false;
}

/**
 * @brief uruchom testy
 */
void executeTests()
{
    unsigned int allTests = 0;
    unsigned int okTests = 0;
    unsigned int failTests = 0;

    /* FILE TESTS */
    string rootTestFold = "../tests/";
    typedef vector< pair< pair<string, string>, string >> test_t;
    typedef map<string, test_t> testCases_t;

    typedef test_t::iterator tIter;
    typedef testCases_t::iterator tCIter;
    testCases_t testCases;

    testCases["nie_sp_war/"] =
    {
        make_pair(make_pair("nsp_gx_v_1.txt", "nsp_gy_v_1.txt"),
                "NIE SPEŁNIA WARUNKÓW WSTĘPNYCH: różna ilość wierzchołków"),
        make_pair(make_pair("nsp_gx_e_1.txt", "nsp_gy_e_1.txt"),
                "NIE SPEŁNIA WARUNKÓW WSTĘPNYCH: różna ilość krawędzi"),
        make_pair(make_pair("nsp_gx_rrs_1.txt", "nsp_gy_rrs_1.txt"),
                "NIE SPEŁNIA WARUNKÓW WSTĘPNYCH: różna liczność stopni wierzchołków 1"),
        make_pair(make_pair("nsp_gx_rrs_1.txt", "nsp_gy_rrs_1.txt"),
                "NIE SPEŁNIA WARUNKÓW WSTĘPNYCH: różna liczność stopni wierzchołków 2"),
    };
    testCases["sp_war_nie_izom/"] =
    {
        make_pair(make_pair("swni_gx_1.txt", "swni_gy_1.txt"),
                "SPEŁNIA WSTĘPNE, NIE JEST IZOMORFICZNY 1"),
        make_pair(make_pair("swni_gx_2.txt", "swni_gy_2.txt"),
                "SPEŁNIA WSTĘPNE, NIE JEST IZOMORFICZNY 2"),
        make_pair(make_pair("swni_gx_3.txt", "swni_gy_3.txt"),
                "SPEŁNIA WSTĘPNE, NIE JEST IZOMORFICZNY 3"),
    };
    testCases["izom/"] =
    {
        make_pair(make_pair("izo_gx_1.txt", "izo_gy_1.txt"),
                "IZOMORFICZNY 1"),
        make_pair(make_pair("izo_gx_2.txt", "izo_gy_2.txt"),
                "IZOMORFICZNY 2"),
        make_pair(make_pair("izo_gx_3.txt", "izo_gy_3.txt"),
                "IZOMORFICZNY 3"),
    };

    map<string, pair<bool, bool>> testCasesFlags;

    testCasesFlags["nie_sp_war/"] = make_pair(false, false);
    testCasesFlags["sp_war_nie_izom/"] = make_pair(true, false);
    testCasesFlags["izom/"] = make_pair(true, true);

    for(tCIter tcit = testCases.begin();
            tcit != testCases.end(); ++tcit) {
        string tfold = rootTestFold + tcit->first;
        for(tIter tit = tcit->second.begin();
                tit != tcit->second.end(); ++tit) {
            string fnameX = tfold + tit->first.first;
            string fnameY = tfold + tit->first.second;
            string testname = tit->second;
            cout << fnameX << ", " << fnameY << endl;

            ++allTests;
            if (runFileTest(fnameX, fnameY, testCasesFlags[tcit->first].first,
                        testCasesFlags[tcit->first].second, allTests,
                        testname)) {
                ++okTests;
            } else {
                ++failTests;
            }
        }

    }

    /* RANDOM TESTS */
    vector<unsigned int> verSizes = {5, 10, 20, 50, 100, 200, 500};
    vector<double> dens = {0.1, 0.2, 0.5};

    vector<unsigned int>::const_iterator vit;
    vector<double>::const_iterator dit;

    for(dit = dens.begin(); dit != dens.end(); ++dit) {
        for(vit = verSizes.begin(); vit != verSizes.end(); ++vit) {
            ++allTests;
            if (runRandomTest((*vit), (*dit), true, allTests,
                        "Test dwóch losowych grafów izomorficznych")) {
                ++okTests;
            } else {
                ++failTests;
            }
        }
    }

    cout << string(80, '=') << endl;
    cout << setw(5) << allTests << " : Wykonanych testów" << endl;
    cout << setw(2) << okTests << "/" << setw(2) << allTests << " : Poprawnych testów" << endl;
    cout << setw(2) << failTests << "/" << setw(2) << allTests << " : Błędnych testów" << endl;
}

/**
 * @brief uruchom program na grafach zapisanych w plikach
 *
 * @param filenameX plik z grafem 1
 * @param filenameY plik z grafem 2
 */
void executeFromFiles(std::string filenameX, std::string filenameY)
{
    try {
        Graph gX, gY;

        readGraph(gX, filenameX, "Graf X");
        readGraph(gY, filenameY, "Graf Y");

        checkIsomorphism(gX, gY);
    } catch (exception e) {
        cout << "BŁĄD: złe dane do wczytywania grafów" << endl;
        cout << "   " << e.what() << endl;
        cout << endl << helpMsg();
    }
}

/**
 * @brief uruchom program na losowych izomorficznych grafach
 *
 * @param v ilosc wierzchołków w grafach losowych
 * @param d gęstość krawędzi w grafach losowych
 */
void executeRandom(unsigned int v, double d)
{
    try {
        Graph gX = Graph::generateRandom(v, d);
        Graph gY; gY.randomIsomorphic(gX);

        checkIsomorphism(gX, gY);
    } catch (exception e) {
        cout << "BŁĄD: złe dane do generowania grafu losowego" << endl;
        cout << "   " << e.what() << endl;
        cout << endl << helpMsg();
    }
}

/**
 * @brief interpretuj argumenty wywołania programu
 *
 * @param argc liczba zmiennych
 * @param argv[] tablica argumentów
 */
void parseInput(int argc, const char *argv[])
{

    if (argc < 2) {
        cout << "BŁĄD: zbyt mała liczba argumentów" << endl << endl;
        cout << helpMsg();
        exit(1);
    }

    string option(argv[1]);

    if (option == "t") {
        executeTests();
        return;
    }

    if (option == "f") {
        if (argc < 4) {
            cout << "BŁĄD: zbyt mała liczba argumentów dla opcji `f'" << endl << endl;
            cout << helpMsg();
            exit(1);
        }
        executeFromFiles(string(argv[2]), string(argv[3]));
        return;
    }

    if (option == "r") {
        if (argc < 4) {
            cout << "BŁĄD: zbyt mała liczba argumentów dla opcji `r'" << endl << endl;
            cout << helpMsg();
            exit(1);
        }
        istringstream issv(argv[2]);
        unsigned int v;
        if (!(issv >> v)) {
            cout << "BŁĄD: liczba wierzchołków musi być liczbą naturalną <- OPCJA `r'" << endl << endl;
            cout << helpMsg();
            exit(1);
        }
        istringstream issd(argv[3]);
        double d;
        if (!(issd >> d)) {
            cout << "BŁĄD: gęstość musi być dodatnią liczbą rzeczywistą z przedziału (0, 1] <- OPCJA `r'" << endl << endl;
            cout << helpMsg();
            exit(1);
        }
        executeRandom(v, d);
        return;
    }

    cout << "BŁĄD: nieznana opcja  `"<< option <<"'" << endl << endl;
    cout << helpMsg();
    exit(1);
}

void plotPoints(unsigned start, unsigned stop, unsigned step, double density)
{
    using std::cout;
    using std::endl;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::duration_cast;

    vector<unsigned int> versizes;
    vector<double> times;

    for(unsigned v = start; v <= stop; v += step){
        try {
            Graph gX = Graph::generateRandom(v, density);
            Graph gY; gY.randomIsomorphic(gX);

            IsomorphismAlgo isoalg(gX, gY);

            duration<double> time_span;
            high_resolution_clock::time_point start = high_resolution_clock::now();
            isoalg.isIsomorphism();
            high_resolution_clock::time_point end = high_resolution_clock::now();
            time_span = duration_cast<duration<double>>(end - start);
            versizes.push_back(v);
            times.push_back(time_span.count());
        } catch (exception e) {
            cout << "BŁĄD: złe dane do generowania grafu losowego" << endl;
            cout << "TEST: FAIL" << cout;
        }
    }
    for(auto v : versizes) {
        cout << v << "," << endl;
    }
    cout << endl << endl;
    cout.precision(3);
    for(auto t : times) {
        cout << fixed << t << "," << endl;
    }
}

/**
 * @brief funkcja main
 *
 * @param argc ilość parametrów programu
 * @param argv[] parametry programu
 *
 * @return zwraca 0 w przypadku poprawnego wykonania, w razie
 * błędnego wartość różną od 0.
 */
int main(int argc, const char *argv[])
{
    parseInput(argc, argv);
    //plotPoints(10, 1000, 10, 0.2);
    return 0;
}

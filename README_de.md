[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AOX4000
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/aox4000/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der AOX4000 ist ein Sauerstoffsensor, der auf dem Prinzip der Fluoreszenzlöschung basiert und den Sauerstoffpartialdruck in der Umgebung misst. Der Sensor verfügt über einen integrierten Luftdrucksensor, der die Werte für Umgebungsluftdruck, Sauerstoffpartialdruck und Sauerstoffkonzentration direkt ausgibt und so ein intuitives Ablesen der Daten ermöglicht. Im Vergleich zu elektrochemischen Sensoren verwendet der AOX4000 verlustfreie, sauerstoffempfindliche Materialien und hat daher eine längere Lebensdauer. Dank der Temperaturkompensation ist kein zusätzliches Kompensationssystem erforderlich. Der AOX4000 ist äußerst stabil und umweltfreundlich, enthält weder Blei noch andere Schadstoffe und ist nahezu unempfindlich gegenüber Störeinflüssen anderer Gase.

LibDriver AOX4000 ist ein voll funktionsfähiger AOX4000-Treiber von LibDriver, der Funktionen wie die Messung von Sauerstoffpartialdruck, Sauerstoffkonzentration, Luftdruck, Temperatur usw. bietet. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver AOX4000-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver AOX4000 UART.

/test enthält den Testcode des LibDriver AOX4000-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver AOX4000-Beispielcode.

/doc enthält das LibDriver AOX4000-Offlinedokument.

/Datenblatt enthält AOX4000-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige UART-Schnittstellenvorlage und stellen Sie Ihren Plattform-UART-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_aox4000_basic.h"

uint8_t res;
uint32_t i;

res = aox4000_basic_init();
if (res != 0)
{
    return 1;
}

...
    
for (i = 0; i < 3; i++)
{
    float oxygen_ppo2_mbar;
    float oxygen_density_percentage;
    float temperature_degree;
    float pressure_mbar;

    /* delay 1000ms */
    aox4000_interface_delay_ms(1000);

    /* read data */
    res = aox4000_basic_read(&oxygen_ppo2_mbar, &oxygen_density_percentage,
                            &temperature_degree, &pressure_mbar);
    if (res != 0)
    {
        (void)aox4000_basic_deinit();

        return 1;
    }

    /* output */
    aox4000_interface_debug_print("aox4000: %d/%d.\n", i + 1, 3);
    aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
    aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
    aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
    aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
    
    ...
}

...
    
/* deinit */
(void)aox4000_basic_deinit();

return 0;
```

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/aox4000/index.html](https://www.libdriver.com/docs/aox4000/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.
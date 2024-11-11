void executeSubMenuFunction(int mainIndex, int subIndex) {
  switch (mainIndex) {
    case 0: // NFC Menu
      switch (subIndex) {
        case 0: nfcReadTag(); break;
        case 1: nfcEmulateTag(); break;
        case 2: nfcBruteForce(); break;
        case 3: nfcSaveToSD(); break;
      }
      break;

    case 1: // Sub-GHz Menu
      switch (subIndex) {
        case 0: subGHzRead(); break;
        case 1: subGHzEmulate(); break;
        case 2: subGHzSaveToSD(); break;
        case 3: subGHzBruteForce(); break;
      }
      break;

    case 2: // Infrared Menu
      switch (subIndex) {
        case 0: irRead(); break;
        case 1: irEmulate(); break;
        case 2: irSaveToSD(); break;
        case 3: irBruteForce(); break;
      }
      break;

    case 3: // Wi-Fi Menu
      switch (subIndex) {
        case 0: wifiPortal(); break;
        case 1: wifiDeauth(); break;
        case 2: wifiScanNetworks(); break;
      }
      break;

    case 4: // BLE Menu
      switch (subIndex) {
        case 0: bleJammer(); break;
        case 1: bleSpoof(); break;
      }
      break;
  }

  // Return to main menu after executing function
  inSubMenu = false;
  showMainMenu();
}

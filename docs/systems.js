/*
 * systems.js
 * Generate the system compatibility table from JSON
 *
 * Copyright (C) 2017 Alyssa Rosenzweig
 * ALL RIGHTS REVERSED
 */

const hw = JSON.parse(require("fs").readFileSync("systems.json").toString());

SYM = symbol => ({"true": "✓", "blob": "☠", "w blob": "☠", "s blob": "☠"})[symbol] || symbol;

const FIELD_WIDTH = 11;

pad = field => (field + Array(FIELD_WIDTH + 1).join(' ')).slice(0, FIELD_WIDTH);

const titles = ["System", "Year", "Weight", "Price", "Chipset", "USB", "Display", "Wi-Fi", "3D", "Video", "EC", "μ-code"];

const flips = Array(titles.length + 1).join(Array(FIELD_WIDTH).join("-") + " ");

console.log(titles.map(pad).join(""));
console.log(flips);

Object.keys(hw.systems).map(sys => {
    const s = hw.systems[sys];
    const chipset = hw.chipsets[s.chipset];

    const fields = [
        sys,
        s.year,
        s.weight,
        s.price,
        s.chipset,
        SYM(chipset.usb),
        SYM(chipset.display),
        SYM(s.wifi),
        SYM(chipset["3d"]),
        SYM(chipset.video),
        SYM(s.ec),
        SYM(chipset.microcode)
    ];

    console.log(fields.map(pad).join(""));
});

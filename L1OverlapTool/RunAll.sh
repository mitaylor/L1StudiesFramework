#!/usr/bin/env bash

date=$1

run_aa() {
    column=$1

    echo "./bin/L1Overlap input/${date}_${column}_2018.root ${date}_${column}_2018"
    echo
}

columns=(HI70kHz HI60kHz HI55kHz HI50kHz HI40kHz HI30kHz HI20kHz HI10kHz HI5kHz)

for column in ${columns[@]}; do
    run_aa ${column}
done

columns=(HI70kHz_Backup35kHzRate HI60kHz_Backup35kHzRate HI50kHz_Backup35kHzRate HI40kHz_Backup35kHzRate HI30kHz_Backup35kHzRate HI20kHz_Backup35kHzRate HI10kHz_Backup35kHzRate HI5kHz_Backup35kHzRate)

for column in ${columns[@]}; do
    run_aa ${column}
done

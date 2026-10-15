# MO-ABM artificial economy (CSS 2026)

Source code of the agent-based artificial economic model used in

> Ogibayashi, S. (2026). Mechanism-Oriented Agent-Based Modeling (MO-ABM): A Methodology for Identifying the Indispensable Model Structures and Causal Mechanisms of Socio-Economic Phenomena. In *Proceedings of the 2026 Conference of the Computational Social Science Society of the Americas*. Springer (to appear).

The model consists of consumers (C), retailers (R), raw-material makers (W, called `Wholesaler` in the code), sole proprietors (S), and the government (G). All transactions are recorded by double-entry bookkeeping, from which balance sheets, income statements, an input–output table, and GDP are computed at every step. The behavioral rules (Eqs. (1)–(3) of the paper) and all other details are described in the ODD document (ODD_MO-ABM-CSS2026.pdf in this repository), which is also provided as supplementary material of the paper.

This repository contains the versions used for the paper. Further development of the model takes place separately.

| Release | Used for | Difference |
|---|---|---|
| `v1.0-CSS2026` | Figs. 4–6 and Table 2 (autonomous economy, income tax, random seeds, number of agents) | Original version: when a consumer cannot afford an item, the purchase loop of that period ends |
| `v1.1-CSS2026` | Fig. 3 (initial prices) | `Consumer.h`: an unaffordable item is excluded and purchasing continues with the remaining items (as described in Sect. 4.2 of the paper); `Producer.h`: the rounding remainder of the production quantity is carried over to the next period, and sole proprietors refer to the sales data of the retail market |

To obtain a specific version, select the tag under *Releases* (or switch the branch/tag selector to the tag) and download the source code.

## Repository structure

```
Project1_20260927.slnx            Visual Studio solution
Project1_20260927/                C++ source (header-based; entry point main.cpp)
FolderForIOFiles/parameter/       Input parameter files Set_parameter_1.csv ... _9.csv
FolderForIOFiles/data/            Output folder (created at run time; not tracked)
results/                          Output data used for the figures and tables of the paper
LICENSE.md                        PolyForm Noncommercial License 1.0.0
```

Main classes: `Consumer.h` (purchase rule, Eq. 1), `Producer.h` (production and pricing rules, Eqs. 2–3), `Retailer.h`, `Wholesaler.h`, `SoleProprietor.h`, `Government.h`, `AreaMarket.h` (market and stock list), `BookKeeper.h` / `Accountant.h` (double-entry bookkeeping), `SimulatorApp.h` (schedule of one step), `SimulatorConfig.h` (reading of parameter files).

## Requirements and build

- Windows 10/11, Visual Studio 2026 (platform toolset v145, C++20). With Visual Studio 2022, retarget the project to toolset v143 (Project > Retarget solution).
- Source files are encoded in UTF-8 with BOM.

1. Open `Project1_20260927.slnx` in Visual Studio.
2. Select the `x64` platform and the `Release` (or `Debug`) configuration.
3. Build the solution and run it (Ctrl+F5).

The program reads and writes files through the relative path `..\FolderForIOFiles\`, so the working directory must be the project folder `Project1_20260927` (the Visual Studio default). Results are written as CSV files under `FolderForIOFiles/data/`.

## Parameter files

Only the first line of each file is read; the second line, where present, gives the column names.

| File | Contents |
|---|---|
| `Set_parameter_1.csv` | run_period_count, consumer_count, retailer_count, wholesaler_count, soleproprietor_count, equipmentmaker_count, bank_count, government_count, retailer_worker_count, wholesaler_worker_count, soleproprietor_worker_count, retailer_executive_count, wholesaler_executive_count, soleproprietor_executive_count |
| `Set_parameter_2.csv` | seed, item_type_id_count_r, item_type_id_count_s, item_type_id_count_w, critical_flag_for_priceup, critical_flag_for_pricedown, producer_demand_delta_min, producer_demand_delta_max, producer_price_alpha |
| `Set_parameter_3.csv` | Producer: sales averaging period n, β, stock ratios, initial quantity ratio, price ranges |
| `Set_parameter_4.csv` | Consumer: rpc (marginal propensity to consume), c_min, initial cash range, base-wage range |
| `Set_parameter_5.csv` | Retailer: initial cash, q_max, q_min, initial price, minimum price, price dispersion |
| `Set_parameter_6.csv` | Raw-material maker (Wholesaler): same items as file 5 |
| `Set_parameter_7.csv` | Sole proprietor: same items as file 5 (price dispersion 0.06) |
| `Set_parameter_8.csv` | Market options |
| `Set_parameter_9.csv` | Government: income tax rate, corporate tax rate, market purchase ratio (mpr), subsidy ratio, initial cash range |

The files included here contain the settings of the last run. The experiments of the paper are reproduced by changing the following items (baseline values in parentheses):

| Experiment (paper) | System | Items to change |
|---|---|---|
| Price equilibrium (Fig. 3) | C–S, S = 600, 800 steps | initial price range in file 7 (250–300, 400–450, 1,200–1,250, 2,000–2,500); r, β, α in files 2–3 |
| Autonomous economy (Fig. 4) | C–R–W–S, R = 30, W = 8, 150 steps | soleproprietor_count in file 1 (0, 100, 300, 600); q_max ranges in files 5 and 7 (qmax_ratio 1.5, 2, 3); base-wage range in file 4 (×1, 1.5, 1.8, 2) |
| Income tax (Fig. 5) | C–S–G, S = 600, 150 steps | tax_rate_income and mpr in file 9; rpc in file 4 (0.8, 0.7); seed in file 2 (500 and five others) |
| Number of agents (Fig. 6) | C–S, 360 steps | soleproprietor_count (= consumer_count) in file 1 (5–600) |

## Output data of the paper

| File in `results/` | Paper |
|---|---|
| `Fig3_initial_price.xlsx` | Fig. 3 |
| `Fig4_autonomous_economy.xlsx`, `Fig4a_S100.xlsx` | Fig. 4 |
| `Fig6_number_of_agents.xlsx` | Fig. 6 |
| `Table2_IO_table.xlsx` | Table 2 |

## License

Copyright (c) 2026 Shigeaki Ogibayashi.

This software is licensed under the [PolyForm Noncommercial License 1.0.0](LICENSE.md). It may be used, modified, and shared freely for noncommercial purposes, including academic research and teaching. Commercial use requires a separate license from the author; please contact ogibayashi@ogi-lab.net.

## Citation

If you use this code, please cite the paper above.

# Dupe Lab 💄

Dupe Lab is a beginner-friendly C++ program that finds lower-cost makeup alternatives using a custom similarity score.

The project is intentionally built without machine learning so that the ranking logic is easy to understand and explain.

## What the program does

1. Loads makeup products from `products.csv`
2. Lets the user choose an original product
3. Asks for a maximum dupe price
4. Filters products from the same makeup category
5. Calculates a similarity score
6. Sorts the matches from best to worst
7. Shows the top three results

## Similarity algorithm

Dupe Lab currently uses this simple scoring system:

- Same color family: 35 points
- Same undertone: 25 points
- Same finish: 20 points
- Lower price than the original: 20 points

Maximum score: 100

Example:

```text
Original:
Rare Beauty Soft Pinch Hope

Candidate:
e.l.f. Camo Dusty Rose

Same color family   +35
Same undertone      +25
Same finish         +20
Lower price         +20

Total               100%
```

## C++ concepts practiced

- `struct`
- `vector`
- functions
- loops
- conditionals
- file input
- CSV parsing
- sorting with `std::sort`
- lambda expressions
- references
- string comparison
- input validation

## Files

```text
Dupe Lab/
├── main.cpp
├── products.csv
└── README.md
```

## Compile and run

```bash
g++ -std=c++17 main.cpp -o dupelab
./dupelab
```

### On Windows if g++ is installed

```powershell
g++ -std=c++17 main.cpp -o dupelab.exe
.\dupelab.exe
```

Keep `products.csv` in the same folder as the compiled program.

## Beginner-friendly development plan

### Version 1
- Read products from CSV
- Choose a product
- Filter by budget
- Rank dupes

### Version 2
- Search products by name instead of number
- Add product categories
- Let users choose which feature matters most

### Version 3
- Add favorites
- Add more products
- Export results to a text file

## Important limitation

This project does not claim that two makeup products are objectively identical. The match percentage is only a score produced by DupeLab's own rules using the dataset attributes.

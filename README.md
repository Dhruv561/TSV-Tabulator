Program which takes in a TSV file, stores the data inside and sorts it based of column numbers, and presents a "medal" count.
Program can take multuple column numbers and will sort with the 1st provided unless there is a tie in which case it will move on to the next one.

Usage:
./project1 <file.tsv 4 2

Example:
./project1 <test0.tsv 4 2

Output (Using previous example):
Stage 1
input tsv data has 6 rows and 5 columns
row 6 is:
   1: Year       2024
   2: Event      Cycling
   3: Gender     Womens
   4: Country    New Zealand
   5: Medal      third: bronze

Stage 2
sorting by "Country",
   then by "Event"
row 1 is:
   1: Year       2024
   2: Event      Cycling
   3: Gender     Womens
   4: Country    China
   5: Medal      first: gold
row 3 is:
   1: Year       2024
   2: Event      Swimming
   3: Gender     Womens
   4: Country    Indonesia
   5: Medal      third: bronze
row 6 is:
   1: Year       2024
   2: Event      Swimming
   3: Gender     Womens
   4: Country    New Zealand
   5: Medal      first: gold


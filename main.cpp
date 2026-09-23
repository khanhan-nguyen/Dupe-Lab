#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Product
{
    string brand;
    string name;
    string category;
    string colorFamily;
    string undertone;
    string finish;
    double price;
};

struct MatchResult
{
    Product product;
    double score;
};

string toLowerCase(string text)
{
    for (char& c : text)
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }

    return text;
}

vector<Product> loadProducts(const string& filename)
{
    vector<Product> products;
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Could not open " << filename << endl;
        return products;
    }

    string line;

    // Skip the header row.
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);

        Product product;
        string priceText;

        getline(ss, product.brand, ',');
        getline(ss, product.name, ',');
        getline(ss, product.category, ',');
        getline(ss, product.colorFamily, ',');
        getline(ss, product.undertone, ',');
        getline(ss, product.finish, ',');
        getline(ss, priceText);

        if (priceText.empty())
        {
            continue;
        }

        product.price = stod(priceText);
        products.push_back(product);
    }

    file.close();
    return products;
}

void printProducts(const vector<Product>& products)
{
    cout << "\nAvailable products:\n\n";

    for (int i = 0; i < static_cast<int>(products.size()); i++)
    {
        cout << i + 1 << ". "
             << products[i].brand << " - "
             << products[i].name << " | "
             << products[i].category << " | $"
             << fixed << setprecision(2) << products[i].price
             << endl;
    }
}

double calculateSimilarity(const Product& original, const Product& candidate)
{
    double score = 0.0;

    // Color family is the most important feature.
    if (toLowerCase(original.colorFamily) == toLowerCase(candidate.colorFamily))
    {
        score += 35.0;
    }

    // Undertone matters because two products with the same color
    // can still look different on the skin.
    if (toLowerCase(original.undertone) == toLowerCase(candidate.undertone))
    {
        score += 25.0;
    }

    // Finish describes whether the product looks matte, dewy, satin, etc.
    if (toLowerCase(original.finish) == toLowerCase(candidate.finish))
    {
        score += 20.0;
    }

    // A dupe should ideally cost less than the original product.
    if (candidate.price < original.price)
    {
        score += 20.0;
    }
    else if (candidate.price == original.price)
    {
        score += 10.0;
    }

    return score;
}

vector<MatchResult> findDupes(
    const vector<Product>& products,
    const Product& original,
    double maxPrice)
{
    vector<MatchResult> matches;

    for (const Product& candidate : products)
    {
        // Do not compare the original product with itself.
        if (candidate.brand == original.brand &&
            candidate.name == original.name)
        {
            continue;
        }

        // Only compare products from the same makeup category.
        if (toLowerCase(candidate.category) != toLowerCase(original.category))
        {
            continue;
        }

        // Respect the user's maximum budget.
        if (candidate.price > maxPrice)
        {
            continue;
        }

        double score = calculateSimilarity(original, candidate);

        MatchResult result;
        result.product = candidate;
        result.score = score;

        matches.push_back(result);
    }

    sort(
        matches.begin(),
        matches.end(),
        [](const MatchResult& a, const MatchResult& b)
        {
            if (a.score == b.score)
            {
                return a.product.price < b.product.price;
            }

            return a.score > b.score;
        });

    return matches;
}

void printTopMatches(
    const Product& original,
    const vector<MatchResult>& matches,
    int amount)
{
    cout << "\n====================================\n";
    cout << "DUPE LAB RESULTS\n";
    cout << "====================================\n";

    cout << "\nOriginal product:\n";
    cout << original.brand << " - " << original.name << endl;
    cout << "Category: " << original.category << endl;
    cout << "Color family: " << original.colorFamily << endl;
    cout << "Undertone: " << original.undertone << endl;
    cout << "Finish: " << original.finish << endl;
    cout << "Price: $" << fixed << setprecision(2) << original.price << endl;

    if (matches.empty())
    {
        cout << "\nNo dupes matched your budget and category.\n";
        return;
    }

    int numberToShow = min(amount, static_cast<int>(matches.size()));

    cout << "\nTop " << numberToShow << " matches:\n";

    for (int i = 0; i < numberToShow; i++)
    {
        const MatchResult& match = matches[i];
        double savings = original.price - match.product.price;

        cout << "\n" << i + 1 << ". "
             << match.product.brand << " - "
             << match.product.name << endl;

        cout << "   Match score: "
             << fixed << setprecision(0)
             << match.score << "%" << endl;

        cout << "   Color family: "
             << match.product.colorFamily << endl;

        cout << "   Undertone: "
             << match.product.undertone << endl;

        cout << "   Finish: "
             << match.product.finish << endl;

        cout << "   Price: $"
             << fixed << setprecision(2)
             << match.product.price << endl;

        if (savings > 0)
        {
            cout << "   You save: $"
                 << fixed << setprecision(2)
                 << savings << endl;
        }
        else
        {
            cout << "   This option does not save money." << endl;
        }
    }
}

int main()
{
    vector<Product> products = loadProducts("products.csv");

    if (products.empty())
    {
        cout << "No products were loaded. Check products.csv." << endl;
        return 1;
    }

    cout << "====================================\n";
    cout << "             DUPE LAB\n";
    cout << "====================================\n";
    cout << "Find a similar makeup product within your budget.\n";

    printProducts(products);

    int choice;
    cout << "\nChoose the number of the product you want to match: ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(products.size()))
    {
        cout << "Invalid product number." << endl;
        return 1;
    }

    double maxPrice;
    cout << "Enter your maximum dupe price: $";
    cin >> maxPrice;

    if (maxPrice < 0)
    {
        cout << "Price cannot be negative." << endl;
        return 1;
    }

    Product original = products[choice - 1];

    vector<MatchResult> matches = findDupes(
        products,
        original,
        maxPrice
    );

    printTopMatches(original, matches, 3);

    return 0;
}

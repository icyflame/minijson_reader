### Example 1: Traverse a given JSON file recursively

> Developed as a toy to understand how minijson works and how to use it when the
> goal is to parse a complete JSON file

```sh
$ g++ -iquote ./ -o print examples/print_all_keys_values.cpp

$ cat basic.json
{
    "k1": "a",
    "k2": 2,
    "k3": true,
    "k4": [
        "a",
        "b"
    ],
    "k5": {
        "u": "z",
        "p": "q"
    }
}

$ ./print basic.json
root > k1 = a
root > k2 = 2
root > k3 = true
root > k4 > 0 = a
root > k4 > 1 = b
root > k4 > k5 > u = z
root > k4 > k5 > p = q
```

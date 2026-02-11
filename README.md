# VARIABLE DECLARATION

```
let age = 21
let name = "antor"
let bachelor = true
let nothing = nil
```

# INPUT

```
display "Enter a number: "
let num = numin()

display "Enter a string: "
let string = strin()
```

# PRINT

```
display age + num
display name + string
```

# CONTROL FLOW

```
if true then
    display "true!"
done

otherwise
    display "false!"
done
```

# WHILE LOOP

```
let i = 1

while i <= 10 do
    display i
    i = i + 1
done
```

# FOR LOOP

```
for (let i = 1 done i <= 10 done i = i + 1)
    display i
done
```

# FUNCTION

```
display "Enter a number to calculate fibonacci number: "
let input = numin()

fn fib(n)

    if n < 2 then
        return n
    done

    otherwise
        return fib(n - 2) + fib(n - 1)
    done

done

input = fib(input)
display input
```

# SCOPING

```
{
    let x = "outer"
    let y = "world"

    {
        let x = "inner"
        display x + y
    }
}
```

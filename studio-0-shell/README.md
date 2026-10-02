# Shell Basics + One Bash Script

This is a exercise to practice core shell pipelines and a simple Bash script.

## Learning Goals
- Use fundamental shell tools (`grep`, `cut`, `sort`, `uniq`, `wc`, `awk`, `echo`, `|`, `head`)
- Write a small Bash script with arguments

---

## Files You Will Produce
- `access.log` - The input file
- `answers.txt` — your commands for Part A
- `log_summary.sh` — your Bash script for Part B

---

## Provided Input: `access.log`

Each line is:

```
IP STATUS PATH
```

Example:

```
10.0.0.1 200 /index.html
10.0.0.2 404 /missing.html
10.0.0.1 200 /products
```

### Create `access.log`

Copy/paste the following into a file named `access.log`:

```text
10.0.0.1 200 /index.html
10.0.0.2 404 /missing.html
10.0.0.3 200 /products
10.0.0.1 200 /products
10.0.0.4 500 /api
10.0.0.2 404 /missing.html
10.0.0.2 200 /index.html
10.0.0.5 301 /old-page
10.0.0.3 200 /index.html
10.0.0.1 404 /missing.html
10.0.0.4 200 /api
10.0.0.5 301 /old-page
10.0.0.6 403 /admin
10.0.0.6 403 /admin
10.0.0.7 200 /products
10.0.0.3 500 /api
10.0.0.2 200 /products
10.0.0.8 200 /index.html
10.0.0.1 200 /index.html
10.0.0.9 404 /missing.html
10.0.0.7 200 /index.html
10.0.0.5 200 /index.html
10.0.0.4 500 /api
10.0.0.3 200 /products
10.0.0.8 404 /missing.html
10.0.0.9 200 /products
10.0.0.2 403 /admin
10.0.0.6 200 /index.html
10.0.0.7 301 /old-page
10.0.0.8 200 /products
```

Or create it using a one-liner:

```bash
cat > access.log << 'EOF'
10.0.0.1 200 /index.html
10.0.0.2 404 /missing.html
10.0.0.3 200 /products
10.0.0.1 200 /products
10.0.0.4 500 /api
10.0.0.2 404 /missing.html
10.0.0.2 200 /index.html
10.0.0.5 301 /old-page
10.0.0.3 200 /index.html
10.0.0.1 404 /missing.html
10.0.0.4 200 /api
10.0.0.5 301 /old-page
10.0.0.6 403 /admin
10.0.0.6 403 /admin
10.0.0.7 200 /products
10.0.0.3 500 /api
10.0.0.2 200 /products
10.0.0.8 200 /index.html
10.0.0.1 200 /index.html
10.0.0.9 404 /missing.html
10.0.0.7 200 /index.html
10.0.0.5 200 /index.html
10.0.0.4 500 /api
10.0.0.3 200 /products
10.0.0.8 404 /missing.html
10.0.0.9 200 /products
10.0.0.2 403 /admin
10.0.0.6 200 /index.html
10.0.0.7 301 /old-page
10.0.0.8 200 /products
EOF
```

---

## Part A — Command-line Tasks (50 points)

Write the command for each task (do **not** write a script). Put your answers in `answers.txt`, one per line.

1. Count total number of lines in `access.log`.
2. Count how many requests have status `404`.
3. Print the **top 3 IPs** by number of requests (format: `IP count`).
4. Print all **unique** paths (sorted).
5. Print the number of requests per status code (format: `STATUS count`, sorted by count descending).

---

## Part B — Script: `log_summary.sh` (20 points)

### Goal
Create a Bash script that summarizes the log.

### Usage
```bash
./log_summary.sh access.log
```

### Required output format (exact headers)
```text
LINES: <N>
STATUS_COUNTS:
<status> <count>
```

### Implementation constraints
- Bash + standard Unix tools only

---

## Submission

Submit the following files on Canvas:
- `access.log`
- `answers.txt`
- `log_summary.sh`

#define _CRT_SECURE_NO_WARNINGS
/**
 * Yeroma Vladyslav (xyerom00)
 * 05/12/2022
 *
 * projekt c.2 IZP
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 *
 */
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h> // strcmp
 /*****************************************************************
  * Ladici makra. Vypnout jejich efekt lze definici makra
  * NDEBUG, napr.:
  *	 a) pri prekladu argumentem prekladaci -DNDEBUG
  *	 b) v souboru (na radek pred #include <assert.h>
  *		#define NDEBUG
  */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

  // vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *	 struct obj_t - struktura objektu: identifikator a souradnice
 *	 struct cluster_t - shluk objektu:
 *		pocet objektu ve shluku,
 *		kapacita shluku (pocet objektu, pro ktere je rezervovano
 *			misto v poli),
 *		ukazatel na pole shluku.
 */

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t* obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

 /*
  Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 */
void sort_cluster(struct cluster_t* c);

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
int init_cluster(struct cluster_t* c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);

	struct obj_t* temp = NULL;

	if (cap > 0)
	{
		temp = malloc(sizeof(struct obj_t) * cap);
	}

	c->obj = temp;
	c->size = 0;
	c->capacity = cap;

	if (temp == NULL && cap != 0)
	{
		fprintf(stderr, "Failed to allocate memory for cluster\n");
		return -1;
	}
	return 0;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t* c)
{
	if (c->obj != NULL)
		free(c->obj);
	init_cluster(c, 0);
}

// Frees all clusters in the array and their objects
void free_all(struct cluster_t** arr, long int length)
{
	for (int i = 0; i < length; i++)
		clear_cluster(*arr + i);
	free(*arr);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t* resize_cluster(struct cluster_t* c, int new_cap)
{
	// TUTO FUNKCI NEMENTE
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);

	if (c->capacity >= new_cap)
		return c;

	const size_t size = sizeof(struct obj_t) * new_cap;

	struct obj_t* arr = (struct obj_t*)realloc(c->obj, size);
	if (arr == NULL)
	{
		arr = malloc(c->obj, size);
		memcpy(arr, c->obj, c->size);
		free(c->obj);
	}

	c->obj = arr;
	c->capacity = new_cap;
	return c;
}

/*

 */
void app_end_cluster(struct cluster_t* c, struct obj_t obj)
{
	if (c->size == c->capacity)
		c = resize_cluster(c, c->size + CLUSTER_CHUNK);

	c->obj[c->size++] = obj;
}

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t* c1, struct cluster_t* c2)
{
	assert(c1 != NULL);
	assert(c2 != NULL);

	for (int i = 0; i < c2->size; i++)
	{
		app_end_cluster(c1, c2->obj[i]);
	}

	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t* carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);

	clear_cluster(carr + idx);

	for (int i = idx; i < narr - 1; i++)
	{
		carr[i] = carr[i + 1];
	}

	return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t* o1, struct obj_t* o2)
{
	assert(o1 != NULL);
	assert(o2 != NULL);

	return sqrt(pow(o1->x - o2->x, 2) + pow(o1->y - o2->y, 2));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t* c1, struct cluster_t* c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	float min_dist = obj_distance(c1->obj, c2->obj);

	for (int i = 0; i < c1->size; i++)
	{
		for (int j = 0; j < c2->size; j++)
		{
			float distance = obj_distance(c1->obj + i, c2->obj + j);
			if (min_dist > distance)
			{
				min_dist = distance;
			}
		}
	}

	return min_dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t* carr, int narr, int* c1, int* c2)
{
	assert(narr > 1);

	float min_dist = cluster_distance(carr, carr + 1);

	for (int i = 0; i < narr - 1; i++)
	{
		for (int j = i + 1; j < narr; j++)
		{
			float distance = cluster_distance(carr + i, carr + j);
			if (distance <= min_dist)
			{
				min_dist = distance;
				*c1 = i;
				*c2 = j;
			}
		}
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void* a, const void* b)
{
	if (((const struct obj_t*)a)->id < ((const struct obj_t*)b)->id)
		return -1;
	if (((const struct obj_t*)a)->id > ((const struct obj_t*)b)->id)
		return 1;
	return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t* c)
{
	qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t* c)
{
	for (int i = 0; i < c->size; i++)
	{
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char* filename, struct cluster_t** arr)
{
	assert(arr != NULL);

	int i;
	FILE* file;
	char head_prefix[7];

	int head_loaded;
	int max_lines_count;

	struct cluster_t* clusters;

	// Open the file for reading.
	file = fopen(filename, "r");

	// Check if file failed to open
	if (file == NULL)
	{
		fprintf(stderr, "Failed to open the input file.\n");
		return -1;
	}

	// Read the header.
	head_loaded = fscanf(file, "%6s%d", head_prefix, &max_lines_count);

	// Error reading header
	if (head_loaded < 2 || head_loaded == EOF)
	{
		fprintf(stderr, "Failed to read header.\n");
		fclose(file);
		return -1;
	}
	// Check if file starts with "count=" 
	else if (strcmp(head_prefix, "count=") != 0)
	{
		fprintf(stderr, "%s is not a valid header prefix.\n");
		fclose(file);
		return -1;
	}
	// Invalid number for max number of lines
	else if (max_lines_count < 1)
	{
		fprintf(stderr, "%i is not a valid number of rows.\n");
		fclose(file);
		return -1;
	}

	// Allocate enough memory to be able to store all clusters
	clusters = malloc(sizeof(struct cluster_t) * max_lines_count);

	//Check if memory allocation failed
	if (clusters == NULL)
	{
		fprintf(stderr, "Could not allocate memory for array of clusters.\n");
		fclose(file);
		return -1;
	}

	// point **arr to clusters
	*arr = clusters;

	// Read the clusters.
	for (i = 0; i < max_lines_count; i++)
	{
		int obj_loaded;
		int id;
		float x, y;
		char line_end_buffer;

		struct cluster_t* cluster;
		struct obj_t object;

		obj_loaded = fscanf(file, "%i %f %f%c", &id, &x, &y, &line_end_buffer);

		// Check validity of values
		if (obj_loaded == EOF)
		{
			fprintf(stderr, "Expected %i objects, however file contains only "
				"%i.\n", max_lines_count, i + 1);
			free_all(arr, i);
			fclose(file);
			return -1;
		}

		// Error reading, incorrect header format.
		if (obj_loaded != 4)
		{
			fprintf(stderr, "Failed to read object.\n");
			free_all(arr, i);
			fclose(file);
			return -1;
		}

		// Line does not end after last float
		if (line_end_buffer != '\n')
		{
			fprintf(stderr, "Line does not end after object.\n"
				"Instead ends in %i", line_end_buffer);
			free_all(arr, i);
			fclose(file);
			return -1;
		}

		// Check uniqueness of IDs
		for (int j = 0; j < i; j++)
		{
			if ((clusters + j)->obj->id == id)
			{
				fprintf(stderr, "All IDs must be unique.\n");
				free_all(arr, i);
				fclose(file);
				return -1;
			}
		}

		// Check whether x and y are whole numbers
		if (x != (int)x || y != (int)y)
		{
			fprintf(stderr, "X and Y components have to be integers.\n");
			free_all(arr, i);
			fclose(file);
			return -1;
		}

		// Check whether x and y are in the range <0;1000>
		if (x < 0 || x > 1000 || y < 0 || y > 1000)
		{
			fprintf(stderr, "X and Y components have to be in the range "
				"<0;1000>.\n");
			free_all(arr, i);
			fclose(file);
			return -1;
		}

		// Tells us on what part of the clusters array we should work
		cluster = clusters + i;

		// Initialize the cluster
		if (init_cluster(cluster, 1) == -1)
		{
			// If allocating memory for the obj failed, clean up and exit
			free_all(arr, i);
			fclose(file);
			return -1;
		}

		object.id = id;
		object.x = x;
		object.y = y;
		app_end_cluster(cluster, object);
	}

	fclose(file);

	return i;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t* carr, int narr)
{
	printf("Clusters:\n");
	for (int i = 0; i < narr; i++)
	{
		printf("cluster %d: ", i);
		print_cluster(&(carr[i]));
	}
}

int main(int argc, char* argv[])
{
	struct cluster_t* clusters;
	int is_clusters_loaded, clusters_count;


	// Only file provided, assume desired number of clusters is 1
	if (argc == 2)
	{
		clusters_count = 1;
	}
	// User also provided desired number of clusters
	else if (argc == 3)
	{
		char* p_end;		// Points to the end of a converted long int.
		long read_value;	// Value read (may not neccesarily be int compatible)

		// Reads and converts the second argument
		read_value = strtol(argv[2], &p_end, 10);

		// Failed reading the int
		if (p_end == argv[2] || *p_end != '\0')
		{
			fprintf(stderr, "Failed to read second argument.\n");
			return -1;
		}

		// Check if read value is int
		if (read_value < INT_MIN || read_value > INT_MAX)
		{
			fprintf(stderr, "Number of desired clusters is too big for Integer.\n");
			return -1;
		}

		// Set the desired clusters from the read value
		clusters_count = read_value;

		// Invalid value
		if (clusters_count <= 0)
		{
			fprintf(stderr, "Number of desired clusters has to be a natural number.\n");
			return -1;
		}
	}
	else
	{
		fprintf(stderr, "Incorrect number of arguments\n");
		return -1;
	}

	is_clusters_loaded = load_clusters(argv[1], &clusters);

	// Some error occured while loading the file
	if (is_clusters_loaded == -1)
	{
		fprintf(stderr, "Could not load clusters\n");
		return -1;
	}

	// User wants us to split cluster into more clusters than we read
	if (clusters_count > is_clusters_loaded)
	{
		fprintf(stderr, "Number of desired clusters has to be less than the number of loaded numbers.\n");
		free_all(&clusters, is_clusters_loaded);
		return -1;
	}

	// Clusters linking
	while (is_clusters_loaded != clusters_count)
	{
		int index1, index2;

		// First find two closest clusters
		find_neighbours(clusters, is_clusters_loaded, &index1, &index2);

		// Merge the second into the first
		merge_clusters(clusters + index1, clusters + index2);

		// And finaly remove the second
		is_clusters_loaded = remove_cluster(clusters, is_clusters_loaded, index2);
	}
	// Show results

	print_clusters(clusters, is_clusters_loaded);

	free_all(&clusters, is_clusters_loaded);

}

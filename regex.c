
void regexMatcher()
{

            size_t size = end - start;

            site = malloc (sizeof (*site) * (size + 1));
            if (site)
            {
               strncpy (site, &str_request[start], size);
               site[size] = '\0';
               printf ("%s\n", site);
               free (site);
            }
         }
/* (5) */
         else if (match == REG_NOMATCH)
         {
            printf ("%s n\'est pas une adresse internet valide\n", str_request);
         }
/* (6) */
         else
         {
            char *text;
            size_t size;

/* (7) */
            size = regerror (err, &preg, NULL, 0);
            text = malloc (sizeof (*text) * size);
            if (text)
            {
/* (8) */
               regerror (err, &preg, text, size);
               fprintf (stderr, "%s\n", text);
               free (text);
            }
            else
            {
               fprintf (stderr, "Memoire insuffisante\n");
               exit (EXIT_FAILURE);
            }
         }
      }
      else
      {
         fprintf (stderr, "Memoire insuffisante\n");
         exit (EXIT_FAILURE);
      }
   }
}

#include<stdio.h>
#include<stdlib.h>

typedef struct flight{
    int flightNumber;
    struct flight* left;
    struct flight* right;
}flight;

flight* createflight(int flightNumber)
{
    flight* newflight = (flight*)malloc(sizeof(flight));
    newflight->flightNumber = flightNumber;
    newflight->left = NULL;
    newflight->right = NULL;
    return newflight;
}

flight* insertflight(flight* root,int flightNumber)
{
    if(root == NULL)
    {
        return createflight(flightNumber);
    }
    if(flightNumber < root->flightNumber)
    {
        root->left = insertflight(root->left,flightNumber);
    }
    else if(flightNumber > root->flightNumber)
    {
        root->right = insertflight(root->right,flightNumber);
    }

    return root;
}

void inOrdertraversal(flight* root)
{
    if(root != NULL)
    {
        inOrdertraversal(root->left);
        printf("flight nUmber: %d\n",root->flightNumber);
        inOrdertraversal(root->right);
    }
}

flight* searchflight(flight* root, int flightNumber)
{
    if(root == NULL || root->flightNumber == flightNumber)
    {
        return root;
    }

    if(flightNumber < root->flightNumber)
    {
        return searchflight(root->left, flightNumber);
    }

    return searchflight(root->right, flightNumber);

}

int main()
{
    flight* root = NULL;

    root = insertflight(root,101);
    root = insertflight(root,201);
    root = insertflight(root,151);
    root = insertflight(root,301);

    printf("flight information: \n");
    inOrdertraversal(root);

    int searchflightNumber;
    printf("\nenter the flight number to search for:");
    scanf("%d",&searchflightNumber);

    
    flight* result = searchflight(root, searchflightNumber);
    if(result != NULL)
    {
        printf("flight %d found!\n",searchflightNumber);

    }
    else
    {
        printf("flight %d not found.\n",searchflightNumber);
    }

    return 0;
}


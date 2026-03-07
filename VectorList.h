#ifndef VECTORLIST_H
#define VECTORLIST_H

#include <iostream>
#include <exception>

using namespace std;

template <typename T> class VectorList
{
    // Lİst veri structu
    struct Node
    {
        T data;
        Node* next;
        Node(T val): data(val), next(nullptr){}
    };
    // classın kendi verileri
    int currentsize;
    // ön tarafı göster
    Node* head;
    // Arka tarafı göster maksat ekle bırak işlemi daha hızlı olsun
    Node* tail;
    
public:
    // Hard copy fonksiyonları
    // verilen listenin initializer olarak çalışmasını sağla VectorList<int> vector1 = new VectorList(vector) gibi
    VectorList(const VectorList& other) : head(nullptr), tail(nullptr), currentsize(0)
    {
            Node* walk = other.head;
            while (walk != nullptr)
            {
                this->push_back(walk->data);
                walk = walk->next;
            }
        }
    // !! && temporary object demekmiş !!
    // Öbür gEçici listenin initializer olarak iş yapmasını sağla VectorList<int>(8, 3) gibi oluşturulabilsin
    VectorList(VectorList&& other) noexcept : head(other.head), tail(other.tail), currentsize(other.currentsize)
    {
        other.head = nullptr;
        other.tail = nullptr;
        other.currentsize = 0;
    }

    VectorList& operator=(VectorList&& other) noexcept
    {
        // Aynı liste verilmesini önle
        if (this != &other)
        {
            // Bu listede olan nodeları sil
            Node* walk = head;
            while(walk)
            {
                Node* next = walk->next;
                delete walk;
                walk = next;
            }
            // bu listenin pointerlarını öbür listenin pointerları ile aynı yeri göstert
            head = other.head;
            tail = other.tail;
            currentsize = other.currentsize;
            // Öbür listenin açık vermesini engelle
            other.head = nullptr;
            other.tail = nullptr;
            other.currentsize = 0;
        }
        return *this;
    }
    // Initializerlar
    VectorList(): head(nullptr), tail(nullptr), currentsize(0){}
    VectorList(int size,T val): head(nullptr), tail(nullptr), currentsize(0)
    {
        try {
            // Saçma girdiler girilmesini önlemek için
            if(size <= 0)
            {
                cout << "Invalid number of entries." << endl;
                return;
            }
            // ilk headi oluştur ondan sonra tail ile ilerle
            head = new Node(val);
            tail = head;
            
            currentsize++;
            // istenilen sayı kadar sonuna ekle
            for(int i = 1; i < size; i++)
            {
                tail->next = new Node(val);
                tail = tail->next;
                currentsize++;
            }
        }catch(const bad_alloc& e){
            cout << "not enough memory from initializer: " << e.what() << endl;
            return;
        }
    }
    // destructor
    ~VectorList()
    {
        // Tüm nodeları güvenli bir şekilde silmek için
        Node *walk = head;
        while(walk != nullptr)
        {
            Node* next_walk = walk->next;
            delete walk;
            walk = next_walk;
        }
    }
    // Sonuna ekleme ve taili son nodeu gösterecek şekilde ayarlama
    void push_back(T val)
    {
        try{
            // Eğer ilk node ise head ve taili o node a göster
            Node* node = new Node(val);
            if(head == nullptr)
            {
                head = tail = node;
            }
            else
            {
                // değilse tailin sonuna ekle taili ilerlet
                tail->next = node;
                tail = node;
            }
            currentsize++;
        }catch(const bad_alloc& e){
            cout<< "not enough memory from push_back(): "<< e.what() << endl;
            return;
        }
    }
    // Sondan eleman silme
    void pop()
    {
        if(head == nullptr)
            return;
        if(head == tail)
        {
            delete head;
            head = tail = nullptr;
            currentsize--;
            return;
        }
        Node* walk = head;
        while(walk->next != tail)
        {
            walk = walk->next;
        }
        
        delete tail;
        tail = walk;
        tail->next = nullptr;
        currentsize--;
    }
    // Verilen indexe göre listeden veri çekme
    T& at(int index)
    {
        if(index < 0 || index >= currentsize)
        {
            cout << "inaccesible memory" << endl;
            throw out_of_range("Out of range access to the memory");
        }
        Node* walk = head;
        for(int i = 0; i < index; i++)
        {
            walk = walk->next;
        }
        return walk->data;
    }
    // Const versiyonu
    const T& at(int index) const
    {
        if(index < 0 || index >= currentsize)
        {
            cout << "inaccesible memory" << endl;
            throw out_of_range("Out of range access to the memory");
        }
        const Node* walk = head;
        for(int i = 0; i < index; i++)
        {
            walk = walk->next;
        }
        return walk->data;
    }
    // Verilen indexe göre veri node silme
    void delete_at(int index)
    {
        // Listede eleman olup olmadığına bak
        if(head == nullptr)
        {
            cout << "empty list" << endl;
            throw out_of_range("Empty list");
        }
        // index kontrol obaaa
        if(index < 0 || index >= currentsize)
        {
            cout << "inaccessible memory" << endl;
            throw out_of_range("Out of range access to the memory");
        }
        
        Node *walk;
        // ilk node istenmişse head üzerinden işlem
        if(index == 0)
        {
            walk = head;
            head = head->next;
            if(head == nullptr)
            {
                tail = nullptr;
            }
            delete walk;
            currentsize--;
            return;
        }
        // herhangi bir yerdeyse 2. bir takip pointeri kullan o pointeri ilerlet
        Node *steady = head;
        
        for(int i = 0; i < index - 1; ++i)
        {
            steady = steady->next;
        }
        // steady istenilen yere gelince walkı steadynin sonrasına daha sonra steadyi walkın sonrasına bağla
        walk = steady->next;
        steady->next = walk->next;
        // son node silinecekse
        if(walk == tail)
        {
            tail = steady;
        }
        // walkı sil
        delete walk;
        currentsize--;
    }
    // Verilen indexe göre node ekleme
    void insert_at(int index, T val)
    {
        // index aralığını kontrol et
        if(index < 0 || index > currentsize)
        {
            cout << "out of range access" << endl;
            throw out_of_range("out of range index");
        }
        try
        {
            // Eğer headin olduğu yerde değişiklilik olacaksa
            if(index == 0)
            {
                Node *node = new Node(val);
                node->next = head;
                head = node;
                if(tail == nullptr)
                    tail = node;
            }
            else
            {
                /* Herhangi bir yerde olacaksa walkı o yere götür ondan sonra oluşturulan nodeu walkın gösterdiği yerin sonraki noduna bağla walkı yaratılan node a bağla*/
                Node *walk = head;
                for(int i = 0; i < index - 1; i++)
                {
                    walk = walk->next;
                }
                Node *node = new Node(val);
                node->next = walk->next;
                walk->next = node;
                // Eğer son node ise taili sonraki oluşturulan node a bağla
                if(walk == tail)
                {
                    tail = node;
                }
            }
            currentsize++;
        }catch(const bad_alloc& e){
            // Depolamada yer biterse
            cout << "not enough space in memory: " << e.what() << endl;
        }
    }
    // listenin büyüklüğünü döndürme
    int size() const {return currentsize;}
};

#endif

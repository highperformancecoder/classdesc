class proxy:
    def __init__(self,target):
        self.target=target

    def __len__(self):
        return self.target.len()

    def __getitem__(self,i):
        return self.target.get(i)

    
